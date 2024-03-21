#include "Request.hpp"
#include "Exceptions.hpp"
WebServer::Request::Request(): body(""), host(""), path(""), method(Methods::Get) {}

WebServer::Request::Request(std::string body, std::string host, std::string path, Methods method): body(body), host(host), path(path), method(method) {}

Request WebServer::Request::newRequest(int fd_request) throw {
    char buffer[1024];
    std::string request;
    int bytes_received = 1;
    while (bytes_received > 0) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0)
            throw new Excp::ErrorRequest(int_request);
        else {
            request = resquest.append(buffer, bytes_received);
        }
    }
        std::cout << "Received: " << buffer << std::endl;
        const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello, World!";
        int bytes_sent = send(client_fd, response, strlen(response), 0);
        if (bytes_sent < 0)
        {
            std::cerr << "Failed to send data to client" << std::endl;
        }
        else
        {
            std::cout << "Sent: " << response << std::endl;
        }
    }
}