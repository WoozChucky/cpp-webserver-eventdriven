//
// Created by Nuno Levezinho Silva on 17/09/2019.
//

#ifndef HTTPROUTER_HPP
#define HTTPROUTER_HPP

#include <Http/HttpRequest.hpp>
#include <Http/HttpResponse.hpp>
#include <map>
#include <string>
#include <functional>

using HttpHandler = std::function<void(HttpRequest* request, HttpResponse* response)> ;

using Route = std::map<HttpMethod, HttpHandler> ;

using Routes = std::map<std::string, Route>;

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
