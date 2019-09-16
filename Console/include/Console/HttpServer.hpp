//
// Created by Nuno Levezinho Silva on 16/09/2019.
//

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <Console/Server.hpp>

class HttpServer  {

public:

    HttpServer();

    void Boot();

private:
    Server* server;
    EventManager * eventManager;

};

#endif //HTTPSERVER_HPP
