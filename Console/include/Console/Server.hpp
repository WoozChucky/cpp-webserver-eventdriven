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

    void Setup();
    void Boot();
    void SetOptions(uint serverPort, std::vector<int> socketOptions, bool sslEnabled);
    void SetEventManager(EventManager* eventManager);

    void OnMessage(OnMessageDelegate delegate);
    void OnClientConnected(OnClientDelegate delegate);
    void OnClientDisconnected(OnClientDelegate delegate);

    void HandleDisconnectionEvent(Context *handle);
    IChannel * GetChannel();

private:
    SocketAddressIn serverAddress;
    int serverSocket;
    uint maxConnections;
    uint port;
    std::vector<int> serverOptions;
    EventManager* manager;
    IChannel* channel;
    MemoryPool* connectionsMemPool;
    bool secure;

    int GetHandle();
    bool SetSocketOption(int option);
    bool GetSocketOption(int option);
    void HandleConnections();

    void HandleNewConnectionEvent();
    void HandleMessageEvent(Context *handle, char* buffer, int bufferSize, ssize_t bytesRead);

    OnMessageDelegate messageDelegate;
    OnClientDelegate clientConnectedDelegate;
    OnClientDelegate clientDisconnectedDelegate;
};

#endif //SERVER_HPP
