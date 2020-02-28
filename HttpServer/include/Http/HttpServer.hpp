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
    explicit HttpServer(ServerConfiguration* configuration);

    void Boot();

    void Handle(const std::string& path, const HttpHandler& handler) const;
    void Handle(const std::string& path, HttpMethod method, HttpHandler handler) const;

private:
    Server* _server;
    EventManager * _eventManager;
    HttpRouter*  _router;
    HttpParser* _parser;
    ServerConfiguration* _configuration;

    HttpRouter* GetRouter() const;
    HttpParser* GetParser() const;
    EventManager* GetEventManager() const;
    Server* GetTransport() const;

    void onClientConnected(SocketContext* ctx);
    void onClientDisconnected(SocketContext* ctx);
    void onClientMessage(SocketContext* ctx, const std::string& messageBuffer);
};

#endif //HTTPSERVER_HPP
