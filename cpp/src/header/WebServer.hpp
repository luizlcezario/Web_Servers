#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#define MAX_EVENTS 10

#include <SocketServer.hpp>
#include <cstdio>
#include <map>
#include <Server.hpp>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


class WebServer
{
    private:
        std::map<std::string, Config::SocketServer *> _socketServers;
        std::map<int, Config::SocketServer *> _sockets;
        int _epoll_fd;
        int _eppollWait();
        struct epoll_event _events[MAX_EVENTS];

    public:
        WebServer();
        ~WebServer();
        void addServerToSocket(std::string ip, Config::Server *server);
        void start();
};

#endif