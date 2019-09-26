//
// Created by Nuno Levezinho Silva on 16/09/2019.
//

#include <Console/HttpServer.hpp>
#include <Console/Http/HttpParser.hpp>
#include <utility>
#include <chrono>

HttpServer::HttpServer() {
    this->server = nullptr;
    this->eventManager = nullptr;
    this->router = new HttpRouter();
    this->parser = new HttpParser(HttpProtocol::V1_1);
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

    this->server->OnMessage([this](Context* ctx, const std::string& messageBuffer) -> void {

#if DEBUG_ENABLED
        if (ctx->socket.handle > 0) {
            fprintf(stdout, "\t[%d][%s:%d] - PACKET\n--- PACKET START\n%s\n--- PACKET END\n",
                    ctx->socket.handle,
                    ctx->socket.address.c_str(),
                    ctx->socket.port,
                    messageBuffer.c_str());
        }
#endif

        auto request = this->parser->RequestFromBuffer(messageBuffer);

        HttpResponse response{};

        auto handler = this->router->GetHandler(request.GetPath());

        std::string responseString;

        if (handler != nullptr) {
            auto startTime = std::chrono::high_resolution_clock::now();

            handler(&request, &response);

            auto endTime = std::chrono::high_resolution_clock::now();

            fprintf(stderr, " Took %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());

            responseString =   "HTTP/1.1 200 OK\r\n"
                               "Date: Mon, 16 Sep 2019 09:10:10 GMT\r\n"
                               "Connection: Keep-Alive\r\n"
                               "Content-Type: application/json\r\n"
                               "Content-Length: 13\r\n"
                               "\r\n"
                               "{\"obj\": true}";
        } else {
            responseString =   "HTTP/1.1 404 Not Found\r\n"
                               "Date: Mon, 16 Sep 2019 09:10:10 GMT\r\n"
                               "Connection: Keep-Alive\r\n"
                               "Content-Type: application/json\r\n"
                               "Content-Length: 0\r\n"
                               "\r\n";
        }

        this->server->GetChannel()->Write(ctx, (void *) responseString.c_str(), responseString.size());

        if (request.GetHeader("Connection").GetValue() == "close")
            this->server->HandleDisconnectionEvent(ctx);

    });

    this->server->Boot();
}

void HttpServer::Handle(const std::string& path, const HttpHandler& handler) {
    this->router->AddRoute(path, handler);
}

void HttpServer::Handle(const std::string& path, HttpMethod method, HttpHandler handler) {
    this->router->AddRoute(path, method, std::move(handler));
}