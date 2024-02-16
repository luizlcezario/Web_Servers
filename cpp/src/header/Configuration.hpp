
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <map>
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>

#include "utils.hpp"
#include "Exceptions.hpp"

#define SLABEL "server"

namespace Config
{
     struct Router {
        bool autoindex;
        bool redirect;
        bool cgi;
        std::string root;
        std::string index;
        std::map<std::string, bool> allow_methods;
        std::string cgiExtension;
        std::string upload;
    };

    typedef struct s_server {
        int port;
        std::string serverName;
        std::map<int, std::string> errorPages;
        size_t bodySize;
        std::map<std::string, struct Router> locations;
    }   Server;

   
    class Configuration
    {
    private:
        std::vector<Server> _config;
        Server *_initServer();
        
    public:
        Configuration();
        ~Configuration();
        void loadFile(std::string filename) throw(Excp::FileNotOpen, Excp::WrongFile, Excp::BadLabel);
    };
};
#endif // CONFIGURATION_HPP
