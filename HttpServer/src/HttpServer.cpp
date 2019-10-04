//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Http/HttpServer.hpp>
#include <Http/HttpParser.hpp>
#include <utility>
#include <chrono>
#include <Socket/SocketOptionsBuilder.hpp>

HttpServer::HttpServer() {
    this->_server = nullptr;
    this->_eventManager = nullptr;
    this->_router = new HttpRouter();
    this->_parser = new HttpParser(HttpProtocol::V1_1);
    this->_configuration = nullptr;
}

HttpServer::HttpServer(ServerConfiguration *configuration) {
    this->_server = nullptr;
    this->_eventManager = nullptr;
    this->_router = new HttpRouter();
    this->_parser = new HttpParser(HttpProtocol::V1_1);
    this->_configuration = configuration;
}

void HttpServer::Boot() {

    if (this->_configuration == nullptr) {

        // default socket server settings
        auto builder = new SocketOptionBuilder();
        this->_configuration = builder
                ->WithReuseAddress()
                ->WithReusePort()
                ->WithKeepAlive()
                ->WithMaxQueuedConnection(100)
                ->WithServerPort(443)
                ->WithSSL(true)
                ->WithCertificate("cert.pem")
                ->WithPrivateKey("key.pem")
                ->Build();
    }

    this->_eventManager = new EventManager();
    this->_server = new Server(this->_configuration);

    this->GetTransport()->SetEventManager(this->GetEventManager());
    this->GetTransport()->Setup();

    this->GetTransport()->OnClientConnected([](SocketContext* ctx) -> void {
        fprintf(stdout, "\t[%d][%s:%d] - CONNECTION\n",
                ctx->Socket.Handle,
                ctx->Socket.Address.c_str(),
                ctx->Socket.Port);
    });

    this->GetTransport()->OnClientDisconnected([](SocketContext* ctx) -> void {
        fprintf(stdout, "\t[%d][%s:%d] - DISCONNECTION\n",
                ctx->Socket.Handle,
                ctx->Socket.Address.c_str(),
                ctx->Socket.Port);
    });

    this->GetTransport()->OnMessage([this](SocketContext* ctx, const std::string& messageBuffer) -> void {

#if DEBUG_ENABLED
        if (ctx->Socket.Handle > 0) {
            fprintf(stdout, "\t[%d][%s:%d] - PACKET\n--- PACKET START\n%s\n--- PACKET END\n",
                    ctx->Socket.Handle,
                    ctx->Socket.Address.c_str(),
                    ctx->Socket.Port,
                    messageBuffer.c_str());
        }
#endif

        auto request = this->GetParser()->RequestFromBuffer(messageBuffer);

        HttpResponse response{};

        auto handler = this->GetRouter()->GetHandler(request.GetPath(), request.GetMethod());

        std::string responseString;

        if (handler != nullptr) {
            auto startTime = std::chrono::high_resolution_clock::now();

            handler(&request, &response);

            auto endTime = std::chrono::high_resolution_clock::now();

            fprintf(stderr, " Took %lld ms\n",
                    std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());

            responseString =   "HTTP/1.1 200 OK\r\n"
                               "Date: Mon, 16 Sep 2019 09:10:10 GMT\r\n"
                               "Connection: Keep-Alive\r\n"
                               "Content-Type: text/html\r\n"
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

        this->GetTransport()->GetChannel()->Write(ctx, (void *) responseString.c_str(), responseString.size());

        if (request.GetHeader("Connection").GetValue() == "close")
            this->GetTransport()->HandleDisconnectionEvent(ctx);

    });

    this->_server->Boot();
}

void HttpServer::Handle(const std::string& path, const HttpHandler& handler) {
    this->_router->AddRoute(path, handler);
}

void HttpServer::Handle(const std::string& path, HttpMethod method, HttpHandler handler) {
    this->_router->AddRoute(path, method, std::move(handler));
}

HttpRouter *HttpServer::GetRouter() {
    return this->_router;
}

HttpParser *HttpServer::GetParser() {
    return this->_parser;
}

EventManager *HttpServer::GetEventManager() {
    return this->_eventManager;
}

Server *HttpServer::GetTransport() {
    return this->_server;
}
