//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <Abstractions/ConcurrentHashMap.hpp>
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
    void Terminate(bool waitForWorkers = false, U32 waitTimeout = 0);

    void Handle(const std::string& path, const HttpHandler& handler) const;
    void Handle(const std::string& path, HttpMethod method, HttpHandler handler) const;

private:
    Server* _server;
    EventManager * _eventManager;
    HttpRouter*  _router;
    HttpParser* _parser;
    ServerConfiguration* _configuration;

    ConcurrentHashMap<U32, SocketContext*>* _connections;
    SocketContext* _lastConnection;

    [[nodiscard]] HttpRouter* GetRouter() const;
    [[nodiscard]] HttpParser* GetParser() const;
    [[nodiscard]] EventManager* GetEventManager() const;
    [[nodiscard]] Server* GetTransport() const;

    void OnClientConnected(SocketContext* ctx);
    void OnClientDisconnected(SocketContext* ctx);
    void OnClientMessage(SocketContext* ctx, const std::string& messageBuffer);

    U16 WriteResponse(SocketContext* ctx, HttpResponse* response);
};

#endif //HTTPSERVER_HPP
