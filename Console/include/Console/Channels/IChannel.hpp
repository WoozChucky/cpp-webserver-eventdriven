//
// Created by Nuno Levezinho Silva on 13/09/2019.
//

#ifndef ICHANNEL_HPP
#define ICHANNEL_HPP

#include <Console/exports.h>

#include <cstdio>

class IChannel {

public:

    virtual void AcceptConnection(SocketHandle handle, Context* outContext) = 0;
    virtual void DisposeConnection(Context* ctx) = 0;
    virtual size_t Write(Context* ctx,void* data, size_t dataLength) = 0;
    virtual std::string Read(Context* ctx) = 0;

};

#endif //ICHANNEL_HPP
