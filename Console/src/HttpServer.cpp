//
// Created by Nuno Levezinho Silva on 16/09/2019.
//

#include <Console/HttpServer.hpp>
#include <utility>

HttpServer::HttpServer() {
    this->server = nullptr;
    this->eventManager = nullptr;
}

void HttpServer::Boot() {

    auto builder = new SocketOptionBuilder();
    auto options = builder
            ->WithReuseAddress()
            ->WithReusePort()
            ->WithKeepAlive()
            ->WithMaxQueuedConnection(100)
            ->WithServerPort(443)
            ->WithSSL(true)
            ->Build();

    this->eventManager = new EventManager();
    this->server = new Server(options);

    this->server->SetEventManager(this->eventManager);
    this->server->Setup();

    this->server->OnClientConnected([](Context* ctx) -> void {
        fprintf(stdout, "\t[%d][%s:%d] - CONNECTION\n",
                ctx->socket.handle,
                ctx->socket.address.c_str(),
                ctx->socket.port);
    });

    this->server->OnClientDisconnected([](Context* ctx) -> void {
        fprintf(stdout, "\t[%d][%s:%d] - DISCONNECTION\n",
                ctx->socket.handle,
                ctx->socket.address.c_str(),
                ctx->socket.port);



    });

    this->server->OnMessage([this](Context* ctx, Message req) -> void {

        if (ctx->socket.handle > 0) {
            fprintf(stdout, "\t[%d][%s:%d] - PACKET\n--- PACKET START\n%s\n--- PACKET END\n",
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

void HttpServer::Handle(std::string path, HttpHandler handler) {

    if (this->router.count(path) < 1) {
        this->router[path] = std::move(handler);
    }

}

void HttpServer::Handle(std::string path, HttpMethod method, HttpHandler handler) {

}
