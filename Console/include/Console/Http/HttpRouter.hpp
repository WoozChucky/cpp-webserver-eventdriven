//
// Created by Nuno Levezinho Silva on 17/09/2019.
//

#ifndef HTTPROUTER_HPP
#define HTTPROUTER_HPP

#include <Console/Http/HttpRequest.hpp>
#include <Console/Http/HttpResponse.hpp>
#include <map>
#include <string>

typedef std::function<void(HttpRequest* request, HttpResponse* response)> HttpHandler;

typedef std::map<HttpMethod, HttpHandler> Route;

typedef std::map<std::string, Route> Routes;

class HttpRouter {

public:
    void AddRoute(const std::string& path, const HttpHandler& handler);
    void AddRoute(const std::string& path, HttpMethod method, HttpHandler handler);

    HttpHandler GetHandler(const std::string& path, HttpMethod method);
    HttpHandler GetHandler(const std::string& path);

private:

    Routes _routes;

};

#endif //HTTPROUTER_HPP
