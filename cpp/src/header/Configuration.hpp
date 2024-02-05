
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <map>
#include <stdexcept>
#include <fstream>
#include <utils.hpp>
#define SLABEL "server"


namespace Config
{
    class Configuration
    {
    private:
        std::map<std::string, std::string> _config;
    public:
        Configuration();
        ~Configuration();
        void loadFile(std::string filename) throw(std::invalid_argument, std::runtime_error);
    };
};
#endif // CONFIGURATION_HPP
