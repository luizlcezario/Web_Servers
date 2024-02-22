package WebServer

import (
	"strconv"
	"strings"
)

type Server struct {
	timeout        int
	clientMaxBytes int
	root           string
	index          []string
	port           []string
	serverName     []string
	errorPages     map[int]string
	locations      map[string]*Location
}

var isErrorPage bool = false
var location string = ""

func NewServer() *Server {
	return &Server{0, 0, "", []string{}, []string{}, []string{}, map[int]string{}, map[string]*Location{}}
}

func convertToSliceOfString(input string) []string {
	input = strings.Trim(input, "[]")
	input = strings.Trim(input, " ")
	split := strings.Split(input, ",")
	return split
}

func addConfig(s *Server, config []string) error {
	var err error
	config[0] = strings.TrimSpace(config[0])
	config[1] = strings.Trim(strings.TrimSpace(config[1]), "\"")
	switch config[0] {
	case "timeout":
		s.timeout, err = strconv.Atoi(config[1])
		println(config[1], s.timeout)
	case "client_max_body_size":
		if strings.HasSuffix(config[1], "K") {
			s.clientMaxBytes, err = strconv.Atoi(config[1][:len(config[1])-1])
			s.clientMaxBytes *= 1024
		} else if strings.HasSuffix(config[1], "M") {
			s.clientMaxBytes, err = strconv.Atoi(config[1][:len(config[1])-1])
			s.clientMaxBytes *= 1024 * 1024
		}
	case "root":
		s.root = config[1]
	case "index":
		s.index = convertToSliceOfString(config[1])
	case "port":
		s.port = convertToSliceOfString(config[1])
	case "server_name":
		s.serverName = convertToSliceOfString(config[1])
	default:
		return nil
	}
	if err != nil {
		return err
	}
	return nil
}

func (s *Server) AddNewLine(line string) error {
	if strings.Contains(line, "=") {
		if !isErrorPage && location == "" {
			addConfig(s, strings.Split(line, "="))
		} else if isErrorPage && location == "" {
			page := strings.Split(line, "=")
			page[0] = strings.TrimSpace(page[0])
			page[1] = strings.Trim(strings.TrimSpace(page[1]), "\"")
			pageNumber, err := strconv.Atoi(page[0])
			if err != nil {
				return err
			}
			s.errorPages[pageNumber] = page[1]
		} else if !isErrorPage && location != "" {
			page := strings.Split(line, "=")
			page[0] = strings.TrimSpace(page[0])
			page[1] = strings.Trim(strings.TrimSpace(page[1]), "\"")
			if _, ok := s.locations[location]; !ok {
				s.locations[location] = NewLocation()
			}
			s.locations[location].AddNewLine(line)
		}
	} else if strings.HasPrefix(line, "[") {
		if strings.Contains(line, "error_page") {
			isErrorPage = true
			location = ""
		} else if strings.Contains(line, "[server.routes.") {
			location = strings.Trim(strings.Split(line, ".")[2], "\"] ")
			isErrorPage = false
		}
	}
	return nil
}

func (s *Server) Print() {
	println("timeout:", s.timeout)
	println("client_max_body_size:", s.clientMaxBytes)
	println("root:", s.root)
	println("server_name:")
	for key, value := range s.serverName {
		println(key, value)
	}
	println("index:")
	for key, value := range s.index {
		println(key, value)
	}
	println("port:")
	for key, value := range s.port {
		println(key, value)
	}
	println("error_pages:")
	for key, value := range s.errorPages {
		println(key, value)
	}

	for key, value := range s.locations {
		print("locations: ")
		println(key)
		value.Print()
	}
}
