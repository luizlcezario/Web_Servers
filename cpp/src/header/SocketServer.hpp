#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <vector>
#include "Server.hpp"
#include "Exceptions.hpp"

namespace Config
{

    typedef struct epoll_event epoll_event;
    class SocketServer
    {
    private:
        std::map<std::string, Server *> servers;
        int listen_fd;
        struct sockaddr_in server_addr;
        int port;
        std::string ipV4;
        epoll_event *_ev;
        void createSocket();
        void bindSocket();
        void listenSocket();

    public:
        SocketServer();
        explicit SocketServer(std::string ip);
        ~SocketServer();
        void addServer(Server *server);
        int getListenFd() const;
        epoll_event* getEv() const;
        void setEv(uint32_t event, int fd);
        void addEpollFd(int epoll_fd);
        static std::string getFullIp(std::string ip);
        
    };
};



#endif