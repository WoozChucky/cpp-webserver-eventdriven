//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <Socket/Server.hpp>
#include <Http/HttpRequest.hpp>
#include <Http/HttpResponse.hpp>
#include <Http/HttpRouter.hpp>
#include <Http/HttpParser.hpp>
#include <map>

class HttpServer  {

public:

    HttpServer();

    void Boot();

    void Handle(const std::string& path, const HttpHandler& handler);
    void Handle(const std::string& path, HttpMethod method, HttpHandler handler);

private:
    Server* server;
    EventManager * eventManager;
    HttpRouter*  router;
    HttpParser* parser;
};

#endif //HTTPSERVER_HPP
