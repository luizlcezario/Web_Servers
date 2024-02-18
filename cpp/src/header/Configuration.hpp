
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "utils.hpp"
#include "Server.hpp"
#include "Exceptions.hpp"


namespace Config
{
   
    class Configuration
    {
    private:
        std::vector<Server *> _config;
        void _populateServer(Server *server, std::string line);
        bool isError;
        bool isLocation;

    public:
        Configuration();
        ~Configuration();
        void loadFile(std::string filename) throw(Excp::FileNotOpen, Excp::WrongFile, Excp::BadLabel);
        std::vector<Server *> getConfig();

    };
};
#endif // CONFIGURATION_HPP
