#include "SocketServer.hpp"

namespace Config
{

    static bool verifyIpNumber(std::string ip) {
        std::vector<std::string> ips = utils::split(ip, ".");
        if (ips.size() == 4) {
            for (std::vector<std::string>::iterator it = ips.begin(); it != ips.end(); it++) {
                if (atoi(it->c_str()) < 0 || atoi(it->c_str()) > 255)
                    return false;
            }
            return true;
        }
        return false;
    }

    std::string SocketServer::getFullIp(std::string ip) {
        std::vector<std::string> ips = utils::split(ip, ":");
        if (ips.size() == 2) {
            if (verifyIpNumber(ips[0]) && atoi(ips[1].c_str()) > 0 && atoi(ips[1].c_str()) < 65535) {
                return ip;
            }
        } else if (ips.size() == 1) {
            if (verifyIpNumber(ips[0])) {
                return ip + (ISROOT ? ":80" : ":8080");
            } else if (atoi(ips[0].c_str()) > 0 && atoi(ips[0].c_str()) < 65535) {
                return "0.0.0.0:" + ip;
            }
        }
        throw Excp::SocketCreation("Invalid format of IP please use 0.0.0.0:80 or 80 or 0.0.0.0");
    }

    SocketServer::SocketServer() : servers(), listen_fd(0), server_addr() , port(ISROOT ? 80 : 8080), ipV4("0.0.0.0"), _ev(new epoll_event)
    {
        this->createSocket();
        this->bindSocket();
        this->listenSocket();
    }

     SocketServer::SocketServer(std::string ip) : servers(), listen_fd(0), server_addr(), _ev(new epoll_event){
        std::vector<std::string> ips = utils::split(ip, ":");
        port = atoi(ips[1].c_str());
        ipV4 = ips[0];
        if (!ISROOT && port < 1024)
            throw Excp::SocketCreation("You must be root to use port < 1024");
        this->createSocket();
        this->bindSocket();
        this->listenSocket();
    }

    SocketServer::~SocketServer()
    {
        delete _ev;
    }

    void SocketServer::createSocket()
    {
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd < 0)
        {
            throw Excp::SocketCreation("Error creating socket");
        }
    }

    void SocketServer::bindSocket()
    {
        int opt = 1;
        if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            throw Excp::SocketBind("-1");
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(ipV4.c_str());
        server_addr.sin_port = htons(port);
        if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            throw Excp::SocketBind("-1");
        }
    }

    void SocketServer::listenSocket()
    {
        if (listen(listen_fd, SOMAXCONN) < 0)
        {
            throw Excp::SocketListen("-1");
        }
    }

    int SocketServer::getListenFd() const
    {
        return listen_fd;
    }

    void SocketServer::addServer(Server *server) {
        std::vector<std::string> serverName = server->getServerName();
        for (std::vector<std::string>::iterator it = serverName.begin(); it != serverName.end(); it++) {
            if (servers.find(*it) == servers.end())
                servers[*it] = server;
            else {
                std::cerr << "Alert :: Server " << *it << " already has a configuration because of this the second will be ignorated as a valid call from the ip: " << ipV4 << ":" << port << std::endl;
            }
        }
    }

    epoll_event SocketServer::getEv() const {
        return _ev;
    }
    void SocketServer::setEv(uint32_t event, int fd) {
        _ev->events = event;
        _ev->data.fd = fd;
    }

    void SocketServer::addEpollFd(int epoll_fd) {
        _ev->events = EPOLLIN;
        _ev->data.fd = listen_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd , &_ev) == -1)
        {
            throw Excp::EpollCreation("Failed to add socket to epoll set");
        }
        
    }
} // namespace Config