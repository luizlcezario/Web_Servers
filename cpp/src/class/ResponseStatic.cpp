#include "Response.hpp"

namespace WebServer
{
    ResponseStatic::ResponseStatic( Config::Server *server, Config::Routes *routes, std::string path, std::string route){
        _server = server;
        _routes = routes;
        _path = path;
        _routeStr = route;
        _autoindex = false;
    };

    void ResponseStatic::createPath(){
        if (utils::starts_with(_path, _routeStr)) {
            _path.erase(0, _routeStr.size());
        }
        if (utils::ends_with(_path, "/")) {
            
            _path += "index.html";
        }
        _file = _root + _path;
    }

    void ResponseStatic::execute()
    {
        createPath();
       std::ifstream _f; 
       std::string file = "";
       std::cout << _file << std::endl;
       _f.open(_file.c_str());
       if (!_f.is_open()) {
            std::cout << "error open file "<< std::endl;
           _status_code = 404;
           _response += "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: 10\n\nError 404\n";
           return;
       }
       std::ostringstream os;
       os << _f.rdbuf();
       file = os.str();
       _response += "HTTP/1.1 200 Ok\n";
       _response += "Content-Type: text/html\n";
       _response += "Content-Length: ";
       _response += std::to_string(file.size());
       _response += "\n\n";
       _response += file; 
    }
    
} // namespace WebServer
