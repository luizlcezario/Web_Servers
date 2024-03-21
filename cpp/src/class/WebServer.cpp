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
            try {
                WebServer::Request req = Request::newRequest(client_fd);

            } catch (Excp::ErrorRequest e) {
                
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