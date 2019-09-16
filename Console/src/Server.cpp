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
    : maxConnections(0), serverSocket(0), serverAddress{}, port(0),
    serverOptions{}, secure(false), manager{nullptr}, channel{nullptr},
      connectionsMemPool{new MemoryPool(sizeof(Context), 3, 100)}
    {

}

void Server::SetOptions(uint serverPort, std::vector<int> socketOptions, bool sslEnabled) {
    this->serverOptions = std::move(socketOptions);
    this->port = serverPort;
    this->secure = sslEnabled;
}

void Server::Setup() {

    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_port = htons(this->port);
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;
    bzero(&(this->serverAddress.sin_zero), sizeof(this->serverAddress.sin_zero));

    this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    for(auto option : this->serverOptions) {
        if (!this->SetSocketOption(option)) {
            fprintf(stdout, "Failed to set socket option %d.\n", option);
        }
    }

    if (this->secure) {
        this->channel = new SecureChannel();
    } else {
        this->channel = new NormalChannel();
    }

}

void Server::Boot() {

    fcntl(this->serverSocket, F_SETFL, O_NONBLOCK); // Mark server socket as non blocking

    if (bind(this->serverSocket, (struct sockaddr *)&this->serverAddress, sizeof(SocketAddress)) == -1) {
        perror("bind()");
        close(this->serverSocket);
    }

    if (listen(this->serverSocket, this->maxConnections) == -1) {
        perror("listen()");
        close(this->serverSocket);
    }

    this->manager->RegisterEvent(this->GetHandle(), EventType::Read, EventAction::Add);

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
        perror("setsockpot()");
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
    ssize_t bytesRead = 0;
    char* buffer = static_cast<char *>(malloc(sizeof(char) * 20));

    while (true) {

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

                this->HandleMessageEvent(ctx, buffer, 20, bytesRead);
            }
        }

    }

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
    auto* ctx = this->connectionsMemPool->Allocate<Context>(); // static_cast<Context *>(this->memPool->GetMemory());

    this->channel->AcceptConnection(this->GetHandle(), this->secure, ctx);

    if (this->clientConnectedDelegate != nullptr)
        this->clientConnectedDelegate(ctx);

    // register read events from client
    this->manager->RegisterEvent(ctx, EventType::Read, EventAction::Add, true);
}

void Server::HandleMessageEvent(Context *ctx, char* buffer, int bufferSize, ssize_t bytesRead) {

    bytesRead = this->channel->Read(ctx, buffer, bufferSize);

    if (bytesRead > 0) {

        if (this->messageDelegate != nullptr) {

            Message message {
                    static_cast<int>(bytesRead), buffer
            };

            this->messageDelegate(ctx, message);

        } else {
            fprintf(stdout, "Received a message, but no message delegate was defined.\n");
        }

    } else {
        fprintf(stdout, "Received a message, but no bytes could be read.\n");
    }

    memset(buffer, 0, bufferSize);

}

IChannel * Server::GetChannel() {
    return this->channel;
}


#pragma clang diagnostic pop
