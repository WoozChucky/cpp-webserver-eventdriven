//
// Created by Nuno Levezinho Silva on 16/09/2019.
//

#include <Console/HttpServer.hpp>
#include <utility>

HttpServer::HttpServer() {
    this->server = nullptr;
    this->eventManager = nullptr;
    this->router = new HttpRouter();
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

        // TODO(Levezinho):
        // Parse the context buffer to a "real" http request (check RFC)
        // either add try-catch to router->GetHandler, or make it return a ptr/nullptr anc check that instead.

        HttpRequest request{};
        HttpResponse response{};

        this->router->GetHandler("/")(&request, &response);


        std::string responseString =   "HTTP/1.1 200 OK\r\n"
                                       "Date: Mon, 16 Sep 2019 09:10:10 GMT\r\n"
                                       "Connection: close\r\n"
                                       "\r\n"
                                       "{\"obj\": true}";

        this->server->GetChannel()->Write(ctx, (void *) responseString.c_str(), responseString.size());
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
