#include "Server.hpp"
namespace Config {

    template <>
    std::vector<int> Server::_parseArray<int>(std::string value) {
        std::string line = utils::trim(value, "[]");
        std::vector<std::string> array = utils::split(line, ',');
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
        std::vector<std::string> array = utils::split(line, ',');
        std::vector<std::string> result;
        for(std::vector<std::string>::iterator it = array.begin(); it != array.end(); it++) {
            result.push_back(utils::trim(*it, "\" \'"));
        }
        return result;
    };
   
    Server::Server() {
    }

    Server::~Server() {
    }

    std::map<int, std::string> Server::getErrorPages() {
        return errorPages;
    }

    void Server::setErrorPages(int code, std::string path) {
        errorPages[code] = path;
    }
    std::map<std::string, std::string> Server::getConfig() {
        return config;
    }

    void Server::setConfig(std::string key, std::string value) {
        config[key] = value;
    }

    std::map<std::string, struct s_router> Server::getLocations() {
        return locations;
    }
    void Server::setLocationsConfig(std::string location, std::string key, std::string value) {
        locations[location].config[key] = value;
    }

    void Server::parseConfig() {
        
        for (std::map<std::string, std::string>::iterator it = config.begin(); it != config.end(); it++) { // Replace 'string' with 'std::string'
            if (it->first == ERRORLABEL) {
                _parseErrorPages(it->second);
            } else if (it->first == SNAMELB) {
                serverName = _parseArray<std::string>(it->second);
            } else if (it->first == LISTENLB) {
                port = _parseArray<int>(it->second);
            } else if (it->first == ROOTLB) {
                root = it->second;
            } else if (it->first == INDEXLB) {
                index = _parseArray<std::string>(it->second);
            } else if (it->first == TIMEOUTLB) {
                timeout = atoi(it->second.c_str());
            } else if (it->first == MBSIZELB) {
                clientMaxBodySize = atoi(utils::trim(it->second, "\"MK").c_str());
                if (it->second.find("M") != std::string::npos)
                    clientMaxBodySize *= 1024 * 1024;
                else if (it->second.find("K") != std::string::npos)
                    clientMaxBodySize *= 1024;
            } else {
                throw Excp::BadLabel(it->first);
            }
        };
        std::cout << *this << std::endl;
    }

    void Server::_parseErrorPages(std::string value) {
        std::string line = utils::trim(value, "{}");
        std::vector<std::string> error_pages = utils::split(line, ',');

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
    utils::printMap(os, server.config);
    os << "+++ ERRO PAGES +++" << std::endl;
    utils::printMap(os, server.errorPages);
    os << "+++ SERVER NAME +++" << std::endl;
    utils::printVector(os, server.serverName);
    os << "+++ PORT +++" << std::endl;
    utils::printVector(os, server.port);
    os << "+++ ROOT +++" << std::endl;
    os << server.root << std::endl;
    os << "+++ INDEX +++" << std::endl;
    utils::printVector(os, server.index);
    os << "+++ TIMEOUT +++" << std::endl;
    os << server.timeout << std::endl;
    os << "+++ CLIENT MAX BODY SIZE +++" << std::endl;
    os << server.clientMaxBodySize << std::endl;
    return os;
}
// namespace Config