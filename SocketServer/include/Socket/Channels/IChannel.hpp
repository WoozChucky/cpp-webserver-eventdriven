//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef ICHANNEL_HPP
#define ICHANNEL_HPP

#include <Socket/Sugar.hpp>

class IChannel {

public:

    virtual void AcceptConnection(SocketHandle handle, SocketContext* outContext) = 0;
    virtual void DisposeConnection(SocketContext* ctx) = 0;

    virtual size_t Write(SocketContext* ctx, Buffer* buffer) = 0;
    virtual std::string Read(SocketContext* ctx) = 0;

    virtual void Terminate() = 0;

};

#endif //ICHANNEL_HPP
