#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#ifdef __APPLE__
    typedef struct kevent poll_event;
#else
    typedef struct epoll_event poll_event;
#endif

#ifdef __APPLE__
    #include <sys/types.h>
    #include <sys/event.h>
    #include <sys/time.h>
#else 
    # include <sys/epoll.h>
#endif

#include "Connection.hpp"
#include "WebServer.hpp"
#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <vector>

#include <algorithm>
    class SocketServer
    {
    private:
        std::map<std::string, Server *> servers;
        int listen_fd;
        struct sockaddr_in server_addr;
        int port;
        std::string ipV4;

        void createSocket();
        void bindSocket();
        void listenSocket();

    public:
        poll_event _ev;
        connection_t _connections;
        SocketServer();
        explicit SocketServer(std::string ip);
        ~SocketServer();
        void addServer(Server *server);
        int getListenFd() const;
        const std::string &getIpV4() const;
        Server *getServer(std::string host);
        void setEv(uint32_t event, int fd);
        void addEpollFd(int epoll_fd);
        void VeifyMethodValid(Request *req);
        static std::string getFullIp(std::string ip);
        void parser(Request *req);
        
    };




#endif