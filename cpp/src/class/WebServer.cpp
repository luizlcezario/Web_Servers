#include "WebServer.hpp"

WebServer::WebServer::WebServer()
{
}

WebServer::WebServer::~WebServer()
{
    for (std::map<int, Config::SocketServer *>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
    {
        delete it->second;
    }
    _sockets.clear();
}

void WebServer::WebServer::addServerToSocket(std::string ip, Config::Server *server)
{
    if (_socketServers.find(ip) == _socketServers.end())
    {
        Config::SocketServer *socket = new Config::SocketServer(ip);
        _socketServers[ip] = socket;
        _sockets[socket->getListenFd()] = socket;
    }
    _sockets[_socketServers[ip]->getListenFd()]->addServer(server);
}


#ifdef __APPLE__
void WebServer::WebServer::start() {
    _epoll_fd = kqueue();
    if (_epoll_fd < 0)
        throw Excp::EpollCreation("Error creating kqueue");
    for (std::map<int, Config::SocketServer *>::iterator it = _sockets.begin(); it != _sockets.end(); it++) { 
        it->second->addEpollFd(_epoll_fd);
    }
    while (_eppollWait());
}

Config::WebServer verifyheaders(Request *req,Config::SocketServer *socket) throw(Excp::ErrorRequest) {
    Config::Server *server = socket->getServer(req->host);
    if (server == NULL)
        throw Excp::ErrorRequest("Host not found");
    

}

int WebServer::WebServer::_eppollWait() {
    static Config::SocketServer *socket = NULL;
    static int conn_sock = -1;
    int num_events = kevent(_epoll_fd, NULL, 0, _events, MAX_EVENTS , NULL);
    if (num_events < 0)
        throw Excp::EpollCreation("Failed to wait for events");
    for (int i = 0; i < num_events; ++i) { 
        int client_fd = _events[i].ident;  
        std::cout << "Event trtrigger try to accept connections: "<< client_fd << std::endl; 
        if (_sockets.find(client_fd) != _sockets.end()) {
            std::cout << "Event trtrigger accept connections"<< client_fd << std::endl;
            socket = _sockets[client_fd];
            conn_sock = accept(socket->getListenFd(), (struct sockaddr *)NULL, NULL);
            if (conn_sock == -1) {
                std::cerr << "connection not accept" << std::endl;
                return 1;
            }
            std::cout << "Event on client coon: " << conn_sock << " "<< socket->getIpV4() << " " << client_fd << std::endl;
            try {
                Request req = Request::newRequest(conn_sock);
                verifyheaders(&req, socket);
                const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello, World!\n\0";
                int bytes_sent = send(conn_sock, response, strlen(response), 0);
                if (bytes_sent < 0)
                {
                    std::cerr << "Failed to send data to client" << strerror(errno) << std::endl;
                    close(conn_sock);
                }
                else
                {
                    std::cout << "Sent: " << response << std::endl;
                }
            } catch (Excp::ErrorRequest e) {
                std::cout << e.what() << std::endl;
            }
            close(conn_sock);
            socket = NULL;
            conn_sock = -1;
            continue;
        }
    }
    return 1;
}
#else 

void WebServer::WebServer::start() {
    _epoll_fd = epoll_create1(0);
	if (_epoll_fd < 0)
        throw Excp::EpollCreation("Error creating epoll");
    for (std::map<int, Config::SocketServer *>::iterator it = _sockets.begin(); it != _sockets.end(); it++) { 
        it->second->addEpollFd(_epoll_fd);
    }
    while (_eppollWait());
}

int WebServer::WebServer::_eppollWait() {
    int num_events = epoll_wait(_epoll_fd, _events, MAX_EVENTS , -1);
    if (num_events < 0)
        throw Excp::EpollCreation("Failed to wait for events");
    for (int i = 0; i < num_events; ++i) { 
        Config::SocketServer *socket;
        int client_fd = _events[i].data.fd;
        if(_sockets.find(client_fd) != _sockets.end()) {
            socket = _sockets[client_fd];
            int conn_sock = accept(socket->getListenFd(), (struct sockaddr *)NULL, NULL);
                if (conn_sock == -1) {
                    perror("accept");
                    return 1;
                }
                socket->setEv(EPOLLIN, conn_sock);
                if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, conn_sock, socket->getEv()) == -1) {
                    perror("epoll_ctl: conn_sock");
                    return 1;
                }
        } else {
            std::cout << "Event on client  "<< socket->getIpV4() << client_fd << std::endl;
            try {
                Request req = Request::newRequest(client_fd);

            } catch (Excp::ErrorRequest& e) {
                std::cout << e.what() << std::endl;
            }
    
            close(client_fd);
            continue;
        }
    }
    return 1;
}

#endif

