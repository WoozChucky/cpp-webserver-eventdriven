//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Socket/Server.hpp>

#include <Socket/Channels/SecureChannel.hpp>
#include <Socket/Channels/NormalChannel.hpp>

#include <Abstractions/Format.hpp>
#include <Abstractions/ThreadPool.hpp>
#include <Abstractions/Timer.hpp>

Server::Server()
        : serverSocket(0), serverAddress{}, manager{nullptr}, handler{nullptr}, channel{nullptr},
          connectionsMemPool{new MemoryPool(sizeof(SocketContext), 3, 100)},
          threadPool{new ThreadPool(1)},
          _configuration{nullptr}, running{false}
{ }

Server::Server(ServerConfiguration* configuration)
        : serverSocket{0}, serverAddress{}, manager{nullptr}, handler{nullptr}, channel{nullptr},
          connectionsMemPool{new MemoryPool(sizeof(SocketContext), 3, 100)},
          threadPool{new ThreadPool(1)},
          running{false}, _configuration{configuration}{
    assert(configuration != nullptr);
}

void Server::Setup() {
    assert(this->_configuration != nullptr);

    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_port = htons(this->_configuration->ServerPort);
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;
    bzero(&(this->serverAddress.sin_zero), sizeof(this->serverAddress.sin_zero));

    this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    for(auto option : this->_configuration->SocketOptions) {
        if (!this->SetSocketOption(option)) {
            TRACE("Failed to set socket option %d.", option);
        }
    }

    if (this->_configuration->SSLEnabled) {
        this->channel = new SecureChannel(
                this->_configuration->SSLCertificatePath,
                this->_configuration->SSLPrivateKeyPath
            );
    } else {
        this->channel = new NormalChannel();
    }

    this->handler = new EventHandler(this->GetHandle());
}

void Server::Boot() {
    assert(this->_configuration != nullptr);

    fcntl(this->serverSocket, F_SETFL, O_NONBLOCK); // Mark server socket as non blocking

    if (bind(this->serverSocket, (struct sockaddr *)&this->serverAddress, sizeof(SocketAddress)) < 0) {
        TRACE("%s %s.", "Socket bind() failed.", strerror(errno));
        close(this->serverSocket);
        throw std::runtime_error(
                    Format::This("Socket bind() failed. %s.", strerror(errno))
                    );
    }

    if (listen(this->serverSocket, this->_configuration->MaxQueuedConnections) < 0) {
        TRACE("%s %s.", "Socket listen() failed.", strerror(errno));
        close(this->serverSocket);
        throw std::runtime_error(
                Format::This("Socket listen() failed. %s.", strerror(errno))
                );
    }

    this->manager->RegisterEvent(this->GetHandle(), EventType::Read, EventAction::Add);

    this->running = true;

    TRACE("Running..%s", ".")

    this->HandleConnections();
}

void Server::Terminate() {

    // stop the main event loop
    this->running = false;

    // unregister server socket from read notifications
    this->manager->RegisterEvent(this->GetHandle(), EventType::Read, EventAction::Delete);

    // close the transport channel
    this->GetChannel()->Terminate();

    // close the server file descriptor
    close(this->GetHandle());
}

void Server::SetConfiguration(ServerConfiguration *configuration) {
    assert(configuration != nullptr);
    this->_configuration = configuration;
}

void Server::SetEventManager(EventManager *eventManager) {
    this->manager = eventManager;
}

void Server::OnMessage(OnMessageDelegate delegate) {
    this->messageDelegate = std::move(delegate);
}

void Server::OnClientConnected(OnClientDelegate delegate) {
    this->clientConnectedDelegate = std::move(delegate);
}

void Server::OnClientDisconnected(OnClientDelegate delegate) {
    this->clientDisconnectedDelegate = std::move(delegate);
}

void Server::HandleDisconnectionEvent(SocketContext *ctx) {

    // Register the disconnection event
    this->manager->RegisterEvent(ctx->Socket.Handle, EventType::Read, EventAction::Delete);

    this->channel->DisposeConnection(ctx);

    this->clientDisconnectedDelegate(ctx);

    this->connectionsMemPool->Release(ctx);
}

IChannel * Server::GetChannel() {
    return this->channel;
}

SocketHandle Server::GetHandle() const {
    return this->serverSocket;
}

bool Server::SetSocketOption(int option) const {

    int optval = 1;
    socklen_t  optlen = sizeof(optval);

    auto error = setsockopt(this->serverSocket, SOL_SOCKET, option, &optval, optlen) < 0;
    if(error) {
        perror("setsockpot()");
        close(this->serverSocket);
    }

    return optval;
}

U16 Server::GetSocketOption(int option) const {

    int optval = 1;
    socklen_t  optlen = sizeof(optval);

    auto error = getsockopt(this->serverSocket, SOL_SOCKET, option, &optval, &optlen) < 0;

    if (error) {
        perror("getsockopt()");
        close(this->serverSocket);
    }

    return optval;
}

void Server::HandleConnections() {

    int maxEvents = 32;
    auto evtList = static_cast<Event *>(malloc(sizeof(Event) * maxEvents));
    int nEvents;

    while (this->running) {

        nEvents = this->manager->GetFiredNotifications(evtList, maxEvents);

        // iterate through all of the events received
        for (auto i = 0; i < nEvents; ++i) {

            auto result = this->handler->DigestEvent(evtList[i]);

            if (result.Context == nullptr) {

                this->HandleNewConnectionEvent();

            } else {

                switch (result.Type)
                {
                    case Read:

                        try {
                            /*
                            this->threadPool->enqueue([this, result]() -> void {
                                //
                            }); */
                            this->HandleMessageEvent(result.Context);
                        } catch (...) {
                            TRACE("%s", "Error HandleMessageEvent");
                        }

                        break;
                    case Write:

                        //TODO(Levezinho): Think about how we can handle this case.
                        TRACE("%s", "Socket Wanted to Write!");

                        break;
                    case Disconnect:

                        this->HandleDisconnectionEvent(result.Context);

                        break;
                    case Weird:
                        TRACE("%s", "Socket Wanted to Write!");
                        break;
                }

            }
        }

    }

    free(evtList);
}

void Server::HandleNewConnectionEvent() {

    // accept the client
    auto* ctx = this->connectionsMemPool->Allocate<SocketContext>();

    auto timer = new Timer(true);

    this->channel->AcceptConnection(this->GetHandle(), ctx);

    TRACE("Accepted Connection took %f ms", timer->Stop() * 0.001)

    if (this->clientConnectedDelegate != nullptr)
        this->clientConnectedDelegate(ctx);

    // register read events from client
    this->manager->RegisterEvent(ctx, EventType::Read, EventAction::Add, true);
}

void Server::HandleMessageEvent(SocketContext *ctx) {

    auto buffer = this->channel->Read(ctx);

    if (!buffer.empty()) {

        if (this->messageDelegate != nullptr) {

            this->messageDelegate(ctx, buffer);

        } else {
            TRACE("%s", "Received a message, but no message delegate was defined.");
        }

    } else {
        TRACE("%s", "Received a message, but no bytes could be read.");
    }

}
