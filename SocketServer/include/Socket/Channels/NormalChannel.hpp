//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef NORMALCHANNEL_HPP
#define NORMALCHANNEL_HPP

#include <Socket/Channels/IChannel.hpp>
#include <Abstractions/MemoryPool.hpp>

class NormalChannel : public IChannel {

public:

    void AcceptConnection(SocketHandle handle, SocketContext* outContext) override;
    void DisposeConnection(SocketContext* ctx) override;
    std::string Read(SocketContext* ctx) override;
    size_t Write(SocketContext* ctx, void* data, size_t dataLength) override;

};

#endif //NORMALCHANNEL_HPP
