#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include "utils.hpp"
#include "Exceptions.hpp"
#define SLABEL "server"
#define ERRORLABEL "error_pages"
#define LOCALABEL "routes"
#define LISTENLB "listen"
#define SNAMELB "server_name" 
#define ROOTLB "root"
#define INDEXLB "index" 
#define TIMEOUTLB "timeout" 
#define MBSIZELB "client_max_body_size"
#define CGILB "cgi"


namespace Config {
    struct s_router {
        std::map<std::string, std::string> config;
    };

     class Server {
        private:
            // struct sockaddr_in server_addr = {};
           

            std::map<std::string, struct s_router> locations;
            void _parseErrorPages(std::string value);
            template <typename T>
            std::vector<T> _parseArray(std::string value);
            
            
        public:
            std::map<int, std::string> errorPages;
            std::map<std::string, std::string> config;
            std::vector<std::string> serverName;
            std::vector<int> port;
            std::string root;
            std::vector<std::string> index;
            int timeout;
            int clientMaxBodySize;

            Server();
            ~Server();
            std::map<int, std::string> getErrorPages();
            void setErrorPages(int code, std::string path);
            std::map<std::string, std::string> getConfig();
            void setConfig(std::string key, std::string value);
            std::map<std::string, struct s_router> getLocations();
            void setLocationsConfig(std::string location, std::string key, std::string  value);
            void parseConfig();
    };
};
    std::ostream &operator<<(std::ostream &os, const Config::Server &server);

#endif