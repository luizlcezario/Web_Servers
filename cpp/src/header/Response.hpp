#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sstream>
#include <fstream>
#include "utils.hpp"
#include "Server.hpp"
#include "Routes.hpp"


namespace WebServer
{

    

    class Response
    {
        protected:
            std::map<std::string, std::string>  _headers;
            std::string                         _response;
            Config::Server                      *_server;
            Config::Routes                      *_routes;
            int                                  _status_code;
            

        public:
            virtual void execute() = 0;
            virtual ~Response() {};
            std::string getResponse() const { return _response; }
            void sendResponse(int fd);
            std::string getMimetype(std::string path);
    };

    class ResponseStatic : public Response
    {
        private:
            std::string _file;

        public:
            ~ResponseStatic() {};
            explicit ResponseStatic( Config::Server *server, Config::Routes *routes){
                _server = server;
                _routes = routes;
            };
            void createPath(std::string root, std::string index, std::string path, std::string route);
            void execute();
    };
    
    
} // namespace WebServer


#endif
