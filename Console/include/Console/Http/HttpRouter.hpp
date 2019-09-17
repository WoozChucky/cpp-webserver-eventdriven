//
// Created by Nuno Levezinho Silva on 17/09/2019.
//

#ifndef HTTPROUTER_HPP
#define HTTPROUTER_HPP

#include <Console/Http/HttpRequest.hpp>
#include <Console/Http/HttpResponse.hpp>
#include <map>

typedef std::function<void(HttpRequest* request, HttpResponse* response)> HttpHandler;

typedef std::map<std::string, HttpHandler> Route;

class HttpRouter {



};

#endif //HTTPROUTER_HPP
