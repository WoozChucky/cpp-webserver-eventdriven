//
// Created by Nuno Levezinho Silva on 16/09/2019.
//

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <Console/Server.hpp>
#include <Console/Http/HttpRequest.hpp>
#include <Console/Http/HttpResponse.hpp>
#include <map>
#include <Console/Http/HttpRouter.hpp>


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
};

#endif //HTTPSERVER_HPP
