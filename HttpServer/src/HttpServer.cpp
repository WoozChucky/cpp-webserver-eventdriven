//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Http/HttpServer.hpp>
#include <chrono>
#include <Socket/SocketOptionsBuilder.hpp>
#include <Http/Exceptions/HttpException.hpp>
#include <Http/Exceptions/NotFoundException.hpp>
#include <Abstractions/Format.hpp>

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

    // Setup the Transport
    this->GetTransport()->SetEventManager(this->GetEventManager());
    this->GetTransport()->Setup();

    // Setup the Transport callbacks
    this->GetTransport()->OnClientConnected([this](SocketContext* ctx) -> void {
        this->OnClientConnected(ctx);
    });

    this->GetTransport()->OnClientDisconnected([this](SocketContext* ctx) -> void {
        this->OnClientDisconnected(ctx);
    });

    this->GetTransport()->OnMessage([this](SocketContext* ctx, const std::string& messageBuffer) -> void {
        this->OnClientMessage(ctx, messageBuffer);
    });

    // Boot the Socket Server
    this->GetTransport()->Boot();
}

void HttpServer::Handle(const std::string& path, const HttpHandler& handler) const
{
    this->GetRouter()->AddRoute(path, handler);
}

void HttpServer::Handle(const std::string& path, HttpMethod method, HttpHandler handler) const
{
    this->GetRouter()->AddRoute(path, method, std::move(handler));
}

HttpRouter *HttpServer::GetRouter() const
{
    return this->_router;
}

HttpParser *HttpServer::GetParser() const
{
    return this->_parser;
}

EventManager *HttpServer::GetEventManager() const
{
    return this->_eventManager;
}

Server *HttpServer::GetTransport() const
{
    return this->_server;
}

void HttpServer::OnClientConnected(SocketContext* ctx) {
    TRACE("[%d][%s:%d] - Connected.",
          ctx->Socket.Handle,
          "",
          // ctx->Socket.Address.c_str(),
          ctx->Socket.Port);
}

void HttpServer::OnClientDisconnected(SocketContext* ctx) {
    TRACE("[%d][%s:%d] - Disconnected",
          ctx->Socket.Handle,
          "",
          //ctx->Socket.Address.c_str(),
          ctx->Socket.Port);
}

void HttpServer::OnClientMessage(SocketContext *ctx, const std::string &messageBuffer) {

    auto startTime = std::chrono::high_resolution_clock::now();

    if (ctx->Socket.Handle > 0) {
        auto logMessage = Format::This(
                "\t[%d][%s:%d] - Sent a message.\n### PACKET START ###\n%s\n### PACKET END ###",
                ctx->Socket.Handle,
                "", // ctx->Socket.Address.c_str(),
                ctx->Socket.Port,
                messageBuffer.data()
        );

        // TRACE("%s", logMessage.data());
    }

    auto request = this->GetParser()->RequestFromBuffer(messageBuffer);

    auto* response = new HttpResponse();

    response->SetProtocol(request->GetProtocol());
    response->AddHeader(HttpHeader("server", "cpp-webserver-eventdriven-1.0"));

    auto handler = this->GetRouter()->GetHandler(request->GetPath(), request->GetMethod());

    try {

        if (handler == nullptr)
            throw NotFoundException(    // No handler has been found, return 404.
                    Format::This(
                            "Resource %s not found",
                            request->GetPath().data()
                            )
                    );

        handler(request, response);

    }  catch (HttpException& ex) {
        response->SetStatusCode(ex.GetStatusCode());
        response->AddHeader(HttpHeader("Connection", "Close"));
        response->AddHeader(HttpHeader("Content-Type", "text/html"));
        response->AddHeader(HttpHeader("Content-Length", "0"));
    } catch (std::runtime_error& error) {
        response->SetStatusCode(INTERNAL_SERVER_ERROR);
        response->SetBody(error.what());
    }

    auto written = this->WriteResponse(ctx, response);

    auto endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = (endTime - startTime);
    auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    TRACE("Request took %ld ms", durationInMs.count())
    TRACE("Written %d bytes as response.", written)

    if (request->GetHeader("Connection") == "close")
        this->GetTransport()->HandleDisconnectionEvent(ctx);
}

U16 HttpServer::WriteResponse(SocketContext *ctx, HttpResponse *response) {

    auto buffer = this->GetParser()->BufferFromResponse(response);

    return this->GetTransport()->GetChannel()->Write(ctx, buffer);
}
