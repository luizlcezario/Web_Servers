#include "Response.hpp"

namespace WebServer
{


    void Response::sendResponse(int fd)
    {
        int send_b = 0;
        for (unsigned long i = 0; i < _response.size(); i += send_b) {
            send_b = send(fd, _response.c_str() + i, _response.size() - i, 0);
            if (send_b < 0)
                std::cerr << "Failed to send data to client" << strerror(errno) << std::endl;
            else
                std::cout << "Sent: " << send_b << std::endl;
        }
    }

    void ResponseStatic::createPath(std::string root, std::string index, std::string path, std::string route) {
        if (utils::starts_with(path, route)) {
            path.erase(0, route.size());
        }
        if (utils::ends_with(path, "/")) {
            path += index;
        }
        _file = root + path;
    }

    void ResponseStatic::execute()
    {
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