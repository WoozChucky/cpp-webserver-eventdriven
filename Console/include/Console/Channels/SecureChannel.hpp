//
// Created by Nuno Levezinho Silva on 13/09/2019.
//

#ifndef SECURECHANNEL_HPP
#define SECURECHANNEL_HPP

#include <Console/Channels/IChannel.hpp>

class SecureChannel : public IChannel {

public:

    void AcceptConnection(SocketHandle handle, bool secureConnection, Context* outContext) override;
    void DisposeConnection(Context* ctx) override;
    size_t Read(Context* ctx, void* data, size_t dataLength) override;
    size_t Write(Context* ctx,void* data, size_t dataLength) override;

private:

    static void InitializeSLLContext(SocketHandle  handle, SSLContext* pContext);

};

#endif //SECURECHANNEL_HPP
