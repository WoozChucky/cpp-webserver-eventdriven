//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Socket/Server.hpp>

#include <Abstractions/Logger.hpp>
#include <Socket/Sugar.hpp>
#include <Socket/Events/EventHandler.hpp>
#include <Socket/Channels/SecureChannel.hpp>
#include <Socket/Channels/NormalChannel.hpp>

#if MACOS
#include <sys/event.h>
#endif

#if LINUX
#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#endif



Server::Server()
        : serverSocket(0), serverAddress{}, manager{nullptr}, handler{nullptr}, channel{nullptr},
          connectionsMemPool{new MemoryPool(sizeof(SocketContext), 3, 100)},
          _configuration{nullptr}, running{false}
{ }

Server::Server(ServerConfiguration* configuration)
        : serverSocket{0}, serverAddress{}, manager{nullptr}, handler{nullptr}, channel{nullptr},
          connectionsMemPool{new MemoryPool(sizeof(SocketContext), 3, 100)},
          running{false}, _configuration{configuration}{
    assert(configuration != nullptr);
}

void Server::SetConfiguration(ServerConfiguration *configuration) {
    assert(configuration != nullptr);
    this->_configuration = configuration;
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
            fprintf(stdout, "Failed to set socket option %d.\n", option);
        }
    }

    if (this->_configuration->SSLEnabled) {
        this->channel = new SecureChannel(this->_configuration->SSLCertificatePath,
                                          this->_configuration->SSLPrivateKeyPath);
    } else {
        this->channel = new NormalChannel();
    }

    this->handler = new EventHandler(this->GetHandle());
}

void Server::Boot() {
    assert(this->_configuration != nullptr);

    fcntl(this->serverSocket, F_SETFL, O_NONBLOCK); // Mark server socket as non blocking

    if (bind(this->serverSocket, (struct sockaddr *)&this->serverAddress, sizeof(SocketAddress)) == -1) {
        perror("bind()");
        close(this->serverSocket);
    }

    if (listen(this->serverSocket, this->_configuration->MaxQueuedConnections) == -1) {
        perror("listen()");
        close(this->serverSocket);
    }

    this->manager->RegisterEvent(this->GetHandle(), EventType::Read, EventAction::Add);

    this->running = true;

    this->HandleConnections();
}

bool Server::SetSocketOption(int option) {

    int optval = 1;
    socklen_t  optlen = sizeof(optval);

    auto error = setsockopt(this->serverSocket, SOL_SOCKET, option, &optval, optlen) < 0;
    if(error) {
        perror("setsockpot()");
        close(this->serverSocket);
    }

    return optval;
}

bool Server::GetSocketOption(int option) {

    int optval = 1;
    socklen_t  optlen = sizeof(optval);

    auto error = getsockopt(this->serverSocket, SOL_SOCKET, option, &optval, &optlen) < 0;

    if (error) {
        perror("getsockopt()");
        close(this->serverSocket);
    }

    return optval;
}

SocketHandle Server::GetHandle() {
    return this->serverSocket;
}

void Server::SetEventManager(EventManager *eventManager) {
    this->manager = eventManager;
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
                            this->HandleMessageEvent(result.Context);
                        } catch (...) {
                            TRACE("%s", "Error HandleMessageEvent");
                        }

                        break;
                    case Write:

                        //TODO(Levezinho): Think about how we can handle this case.

                        break;
                    case Disconnect:

                        this->HandleDisconnectionEvent(result.Context);

                        break;
                }

            }
        }

    }

    free(evtList);
}

void Server::HandleDisconnectionEvent(SocketContext *ctx) {

    // Register the disconnection event
    this->manager->RegisterEvent(ctx->Socket.Handle, EventType::Read, EventAction::Delete);

    this->channel->DisposeConnection(ctx);

    this->clientDisconnectedDelegate(ctx);

    this->connectionsMemPool->Release(ctx);
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

void Server::HandleNewConnectionEvent() {

    // accept the client
    auto* ctx = this->connectionsMemPool->Allocate<SocketContext>();

    this->channel->AcceptConnection(this->GetHandle(), ctx);

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

IChannel * Server::GetChannel() {
    return this->channel;
}
