//
// Created by Nuno Levezinho Silva on 13/09/2019.
//

#ifndef NORMALCHANNEL_HPP
#define NORMALCHANNEL_HPP

#include <Console/Channels/IChannel.hpp>

class NormalChannel : public IChannel {

public:

    void AcceptConnection(SocketHandle handle, Context* outContext) override;
    void DisposeConnection(Context* ctx) override;
    std::string Read(Context* ctx) override;
    size_t Write(Context* ctx,void* data, size_t dataLength) override;

};

#endif //NORMALCHANNEL_HPP
