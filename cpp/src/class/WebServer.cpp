#include "WebServer.hpp"

WebServer::WebServer()
{
}

WebServer::~WebServer()
{
    for (std::map<int, Config::SocketServer *>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
    {
        delete it->second;
    }
    _sockets.clear();
}

void WebServer::addServerToSocket(std::string ip, Config::Server *server)
{
    if (_socketServers.find(ip) == _socketServers.end())
    {
        Config::SocketServer *socket = new Config::SocketServer(ip);
        _socketServers[ip] = socket;
        _sockets[socket->getListenFd()] = socket;
    }
    _sockets[_socketServers[ip]->getListenFd()]->addServer(server);
}

int WebServer::_eppollWait() {
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
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received < 0)
            {
                std::cerr << "Failed to receive data from client" << std::endl;
            }
            else if (bytes_received == 0)
            {
                std::cout << "Client disconnected" << std::endl;
            }
            else
            {
                std::cout << "Received: " << buffer << std::endl;
                const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello, World!";
                int bytes_sent = send(client_fd, response, strlen(response), 0);
                if (bytes_sent < 0)
                {
                    std::cerr << "Failed to send data to client" << std::endl;
                }
                else
                {
                    std::cout << "Sent: " << response << std::endl;
                }
            }
            close(client_fd);
            continue;
        }
    }
    return 1;
}

void WebServer::start() {
    _epoll_fd = epoll_create1(0);
	if (_epoll_fd < 0)
        throw Excp::EpollCreation("Error creating epoll");
    for (std::map<int, Config::SocketServer *>::iterator it = _sockets.begin(); it != _sockets.end(); it++) { 
        it->second->addEpollFd(_epoll_fd);
    }
    while (_eppollWait());
   
}