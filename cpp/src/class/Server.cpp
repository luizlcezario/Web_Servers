#include "Server.hpp"
namespace Config {

    template <>
    std::vector<int> Server::_parseArray<int>(std::string value) {
        std::string line = utils::trim(value, "[]");
        std::vector<std::string> array = utils::split(line, ",");
        std::vector<int> result;
        for(std::vector<std::string>::iterator it = array.begin(); it != array.end(); it++) {
            result.push_back(atoi(utils::trim(*it, "\" \'").c_str()));
        }
        return result;
    };

    // Especialização para std::string
    template <>
    std::vector<std::string> Server::_parseArray<std::string>(std::string value) {
        std::string line = utils::trim(value, "[]");
        std::vector<std::string> array = utils::split(line, ",");
        std::vector<std::string> result;
        for(std::vector<std::string>::iterator it = array.begin(); it != array.end(); it++) {
            result.push_back(utils::trim(*it, "\" \'"));
        }
        return result;
    };
   
    Server::Server() : locations(), errorPages(), config(), serverName(), port(), index(), root(""), timeout(0), clientMaxBodySize(0){
    }

    Server::~Server() {
        for (std::map<std::string, Routes *>::iterator it = locations.begin(); it != locations.end(); it++) {
            delete it->second;
        }
    }

    void Server::setErrorPages(int code, std::string path) {
        errorPages[code] = path;
    }
    const std::map<int, std::string>& Server::getErrorPages() const{
        return errorPages;
    }

    const std::map<std::string, std::string>& Server::getConfig() const{
        return config;
    }

    const std::vector<std::string>& Server::getServerName() const{
        return serverName;
    }

    const std::vector<std::string>& Server::getPort() const{
        return port;
    }

    const std::string& Server::getRoot() const{
        return root;
    }

    const std::vector<std::string>& Server::getIndex() const{
        return index;
    }

    int Server::getTimeout() const{
        return timeout;
    }

    int Server::getClientMaxBodySize() const{
        return clientMaxBodySize;
    }

    void Server::setConfig(std::string key, std::string value) {
        config[key] = value;
    }

    const std::map<std::string, Config::Routes *>& Server::getLocations() const{
        return locations;
    }

    void Server::setLocationsConfig(std::string location, std::string key, std::string value) {
        if (locations.find(location) == locations.end()) {
            locations[location] = new Routes();
        }
        locations[location]->setConfig(key, value);
    }

    void Server::parseConfig() {
        for (std::map<std::string, std::string>::iterator it = config.begin(); it != config.end(); it++) { // Replace 'string' with 'std::string'
            std::string key = utils::trim(it->first);
            if (key == ERRORLABEL) {
                _parseErrorPages(it->second);
            } else if (key == SNAMELB) {
                serverName = _parseArray<std::string>(it->second);
            } else if (key == LISTENLB) {
                port = _parseArray<std::string>(it->second);
            } else if (key == ROOTLB) {
                root = it->second;
            } else if (key == INDEXLB) {
                index = _parseArray<std::string>(it->second);
            } else if (key == TIMEOUTLB) {
                timeout = atoi(it->second.c_str());
            } else if (key == MBSIZELB) {
                clientMaxBodySize = atoi(utils::trim(it->second, "\"MK").c_str());
                if (it->second.find("M") != std::string::npos) {
                    clientMaxBodySize *= 1024 * 1024;
                } else if (it->second.find("K") != std::string::npos) {
                    clientMaxBodySize *= 1024;
                }
            } else {
                std::cout << key << "." << std::endl;
                throw Excp::BadLabel(key);
            }
        };
    }

    void Server::_parseErrorPages(std::string value) {
        std::string line = utils::trim(value, "{}");
        std::vector<std::string> error_pages = utils::split(line, ",");

        for(std::vector<std::string>::iterator it = error_pages.begin(); it != error_pages.end(); it++) {
            std::string code = utils::trim(it->substr(0, it->find("=")));
            std::string path = utils::trim(it->substr(it->find("=") + 1));
            errorPages[atoi(code.c_str())] = path;
        }
    }

} 
std::ostream &operator<<(std::ostream &os, const Config::Server &server) {
    os << "++++++++++ NOVO SERVER +++++++++++++++" << std::endl;
    os << "+++ Config +++" << std::endl;
    utils::printMap(os, server.getConfig());
    os << "+++ ERRO PAGES +++" << std::endl;
    utils::printMap(os, server.getErrorPages());
    os << "+++ SERVER NAME +++" << std::endl;
    utils::printVector(os, server.getServerName());
    os << "+++ PORT +++" << std::endl;
    utils::printVector(os, server.getPort());
    os << "+++ ROOT +++" << std::endl;
    os << server.getRoot() << std::endl;
    os << "+++ INDEX +++" << std::endl;
    utils::printVector(os, server.getIndex());
    os << "+++ TIMEOUT +++" << std::endl;
    os << server.getTimeout() << std::endl;
    os << "+++ CLIENT MAX BODY SIZE +++" << std::endl;
    os << server.getClientMaxBodySize() << std::endl;
    os << "+++ LOCATIONS +++" << std::endl;
    for (std::map<std::string, Config::Routes *>::const_iterator it = server.getLocations().begin(); it != server.getLocations().end(); it++) {
        os << "Location: " << it->first << std::endl << *(it->second) << std::endl;
    }
    return os;
}
// namespace Config