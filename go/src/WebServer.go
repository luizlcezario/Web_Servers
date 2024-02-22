package WebServer

import (
	"fmt"
	"os"
	"strings"
)

type WebServer struct {
	servers []*Server
}

func NewWebServer(filename string) (*WebServer, error) {
	if dat, err := os.ReadFile(filename); err != nil {
		return nil, err
	} else {
		lines := strings.Split(string(dat), "\n")
		servers := []*Server{}
		var server *Server = nil
		for _, line := range lines {
			line = strings.TrimSpace(line)
			if strings.Contains(line, "#") {
				line = line[:strings.Index(line, "#")]
			}
			if len(line) == 0 {
				continue
			}
			if strings.HasPrefix(line, "[[") && strings.HasSuffix(line, "]]") && strings.Contains(line, "server") {
				println(line)
				if server != nil {
					isErrorPage = false
					location = ""
					servers = append(servers, server)
				}
				server = NewServer()
			} else {
				err := server.AddNewLine(line)
				if err != nil {
					return nil, err
				}
			}
		}
		servers = append(servers, server)
		return &WebServer{servers: servers}, nil
	}
}

func (s *WebServer) Print() {
	println("Print WebServers:")
	for idx, server := range s.servers {
		fmt.Printf("server Number: %d\n", idx)
		server.Print()
	}
}
