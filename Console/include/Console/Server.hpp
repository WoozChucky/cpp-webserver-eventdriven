//
// Created by Nuno Levezinho Silva on 11/09/2019.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#include <Console/SocketOptionBuilder.hpp>
#include <Console/exports.h>
#include <Console/EventManager.hpp>
#include <Console/Channels/IChannel.hpp>
#include <Console/Common/MemoryPool.hpp>

typedef std::function<void(Context* ctx, Message message)> OnMessageDelegate;
typedef std::function<void(Context* ctx)> OnClientDelegate;

class Server {

public:

    Server();
    explicit Server(ServerConfiguration* configuration);

    void Setup();
    void Boot();
    void SetConfiguration(ServerConfiguration* configuration);
    void SetEventManager(EventManager* eventManager);

    void OnMessage(OnMessageDelegate delegate);
    void OnClientConnected(OnClientDelegate delegate);
    void OnClientDisconnected(OnClientDelegate delegate);

    void HandleDisconnectionEvent(Context *handle);
    IChannel * GetChannel();

private:
    SocketAddressIn serverAddress;
    SocketHandle serverSocket;
    EventManager* manager;
    IChannel* channel;
    MemoryPool* connectionsMemPool;
    MemoryPool* readBufferMemPool;
    bool running;

    ServerConfiguration* _configuration;

    int GetHandle();
    bool SetSocketOption(int option);
    bool GetSocketOption(int option);
    void HandleConnections();

    void HandleNewConnectionEvent();
    void HandleMessageEvent(Context *handle, char** buffer, int bufferSize);

    OnMessageDelegate messageDelegate;
    OnClientDelegate clientConnectedDelegate;
    OnClientDelegate clientDisconnectedDelegate;
};

#endif //SERVER_HPP
