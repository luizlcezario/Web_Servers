#include "Request.hpp"
#include <sys/types.h>
#include "Exceptions.hpp"
WebServer::Request::Request(): body(""), host(""), path(""), method(GET) {}

WebServer::Request::Request(std::string body, std::string host, std::string path, Methods method): body(body), host(host), path(path), method(method) {}

WebServer::Methods WebServer::getMethod(std::string method) {
    if (method == "GET")
        return GET;
    else if (method == "POST")
        return POST;
    else if (method == "DELETE")
        return DELETE;
    else
        throw Excp::ErrorRequest("Method not supported");
}

WebServer::Request WebServer::Request::newRequest(int fd_request) throw(Excp::SocketCreation) {
    char buffer[1024];
    std::string requestContent = "";
    while (bytes_received > 0) {
        std::cout << "Receiving ... "  << std::endl;
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(fd_request, buffer, sizeof(buffer) - 1, 0);
        std::cout << "Received: " << bytes_received << std::endl;
        if (bytes_received < 0)
            throw Excp::ErrorRequest("error receiving data from client" + std::to_string(fd_request));
        else 
            requestContent = requestContent.append(buffer, bytes_received);
    }
    Request req = Request();
    std::string line = strtok(requestContent.c_str(), "\n");
    req.method = strtok(line.c_str(), " ");
    req.path = strtok(line, " ");
    req.host = strtok(requestContent.c_str(), " ");
    std::cout << "Method: " << req.method << std::endl;
    return req;
}
