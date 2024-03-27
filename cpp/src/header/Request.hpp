#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "Exceptions.hpp"
#include "utils.hpp"
#include <string>
#include <sys/socket.h>
#include <iostream>
#include <sys/types.h>

#define HOST "Host"
#define CONTENT_LENGTH "Content-Length"
#define CONTENT_TYPE "Content-Type"
#define REQUEST "Request"

namespace WebServer {

    typedef enum e_METHODS {
        GET = 1,
        POST = 2,
        DELETE = 3,
    } Methods;

    Methods getMethodE(std::string method);

    class Request {
        private:
        
        std::string body;
        std::string host;
        std::string path;
        Methods method;
        int body_length;
        std::string content_type;

        public:
        Request();
        Request(std::string body, std::string host, std::string path, Methods method);
        std::string getBody() const { return body; }
        std::string getHost() const { return host; }
        std::string getPath() const { return path; }
        std::string getMethod() const;

        static Request newRequest(int fd_request) throw(Excp::SocketCreation);
    };

    

};

std::ostream &operator<<(std::ostream &os, const WebServer::Request &req);


#endif