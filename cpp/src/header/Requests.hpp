#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include <Webserver>

using WebServer {

    typedef enum e_METHODS {
        GET = 1,
        POST = 2,
        DELETE = 3,
    } Methods;


    class Request {
        private:
        std::string body;
        std::string host;
        std::string path;
        Methods method;

        public:
        Request();
        Request(std::string body, std::string host, std::string path, Methods method);

        static Request newRequest(int fd_request) throw;
    };

};


#endif