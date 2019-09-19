//
// Created by Nuno Levezinho Silva on 11/09/2019.
//

#include <Console/Server.hpp>
#include <string>
#include <utility>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/event.h>
#include <Console/Channels/IChannel.hpp>
#include <Console/Channels/SecureChannel.hpp>
#include <Console/Channels/NormalChannel.hpp>
#include <openssl/err.h>

Server::Server()
    : serverSocket(0), serverAddress{}, manager{nullptr}, channel{nullptr},
      connectionsMemPool{new MemoryPool(sizeof(Context), 3, 100)},
      _configuration{nullptr}, running{false}
    {

}

Server::Server(ServerConfiguration* configuration)
    : serverSocket{0}, serverAddress{}, manager{nullptr}, channel{nullptr},
    connectionsMemPool{new MemoryPool(sizeof(Context), 3, 100)},
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
        this->channel = new SecureChannel();
    } else {
        this->channel = new NormalChannel();
    }

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

int Server::GetHandle() {
    return this->serverSocket;
}

void Server::SetEventManager(EventManager *eventManager) {
    this->manager = eventManager;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void Server::HandleConnections() {

    int maxEvents = 32;
    auto evtList = static_cast<Event *>(malloc(sizeof(Event) * maxEvents));
    int nEvents = 0;

    while (this->running) {

        nEvents = this->manager->GetFiredNotifications(evtList, maxEvents);

        // iterate through all of the events received
        for (auto i = 0; i < nEvents; ++i) {

            auto event = evtList[i];

            if (event.flags & (unsigned)EV_EOF) { // disconnection event

                auto ctx = reinterpret_cast<Context *>(event.udata);

                this->HandleDisconnectionEvent(ctx);

            } else if (event.ident == this->GetHandle()) { // new client connection event

                this->HandleNewConnectionEvent();

            } else if (event.filter == EventType::Read) { // new client message

                auto ctx = reinterpret_cast<Context *>(event.udata);

                try {
                    this->HandleMessageEvent(ctx);
                } catch (...) {
                    fprintf(stderr, "Error HandleMessageEvent\n");
                }
            }
        }

    }

    free(evtList);
}

void Server::HandleDisconnectionEvent(Context *ctx) {

    // Register the disconnection event
    this->manager->RegisterEvent(ctx->socket.handle, EventType::Read, EventAction::Delete);

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
    auto* ctx = this->connectionsMemPool->Allocate<Context>();

    this->channel->AcceptConnection(this->GetHandle(), ctx);

    if (this->clientConnectedDelegate != nullptr)
        this->clientConnectedDelegate(ctx);

    // register read events from client
    this->manager->RegisterEvent(ctx, EventType::Read, EventAction::Add, true);
}

void Server::HandleMessageEvent(Context *ctx) {

    auto buffer = this->channel->Read(ctx);

    if (!buffer.empty()) {

        if (this->messageDelegate != nullptr) {

            this->messageDelegate(ctx, buffer);

        } else {
            fprintf(stdout, "Received a message, but no message delegate was defined.\n");
        }

    } else {
        fprintf(stdout, "Received a message, but no bytes could be read.\n");
    }

}

IChannel * Server::GetChannel() {
    return this->channel;
}

#pragma clang diagnostic pop
