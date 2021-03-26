//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#include <Abstractions/MemoryPool.hpp>
#include <Socket/Sugar.hpp>
#include <Socket/Channels/IChannel.hpp>
#include <Events/EventManager.hpp>
#include <Events/EventHandler.hpp>


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

    void HandleDisconnectionEvent(SocketContext *handle);
    IChannel * GetChannel();

private:
    SocketAddressIn serverAddress;
    SocketHandle serverSocket;
    EventManager* manager;
    EventHandler* handler;
    IChannel* channel;
    MemoryPool* connectionsMemPool;
    bool running;

    ServerConfiguration* _configuration;

    SocketHandle GetHandle() const;
    bool SetSocketOption(int option) const;
    bool GetSocketOption(int option) const;
    void HandleConnections();

    void HandleNewConnectionEvent();
    void HandleMessageEvent(SocketContext *handle);

    OnMessageDelegate messageDelegate;
    OnClientDelegate clientConnectedDelegate;
    OnClientDelegate clientDisconnectedDelegate;
};

#endif //SERVER_HPP
