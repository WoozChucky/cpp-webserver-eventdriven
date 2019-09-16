//
// Created by Nuno Levezinho Silva on 16/09/2019.
//

#include <Console/HttpServer.hpp>

HttpServer::HttpServer() {
    this->server = nullptr;
}

void HttpServer::Boot() {

    auto builder = new SocketOptionBuilder();
    auto options = builder
            ->WithReuseAddress()
            ->WithReusePort()
            ->WithKeepAlive()
            ->Build();

    this->eventManager = new EventManager();
    this->server = new Server();

    this->server->SetOptions(443, options, true);
    this->server->SetEventManager(this->eventManager);
    this->server->Setup();

    this->server->OnClientConnected([](Context* ctx) -> void {
        fprintf(stdout, "Client %d has connected from %s:%d\n",
                ctx->socket.handle,
                ctx->socket.address.c_str(),
                ctx->socket.port);
    });

    this->server->OnClientDisconnected([](Context* ctx) -> void {
        fprintf(stdout, "Client %d has disconnected\n", ctx->socket.handle);



    });

    this->server->OnMessage([this](Context* ctx, Message req) -> void {

        fprintf(stdout, "Client %d has sent a message.\n", ctx->socket.handle);

        if (ctx->socket.handle > 0) {
            fprintf(stdout, "\t[%d][%s:%d]\n%s\n",
                    ctx->socket.handle,
                    ctx->socket.address.c_str(),
                    ctx->socket.port,
                    req.buffer.data);
        }

        std::string response = "HTTP/1.1 200 OK\r\n"
                               "Date: Mon, 16 Sep 2019 09:10:10 GMT\r\n"
                               "Connection: close\r\n"
                               "\r\n"
                               "{\"obj\": true}";

        this->server->GetChannel()->Write(ctx, (void *) response.c_str(), response.size());
        this->server->HandleDisconnectionEvent(ctx);

    });

    this->server->Boot();
}
