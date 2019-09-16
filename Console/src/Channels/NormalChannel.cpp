//
// Created by Nuno Levezinho Silva on 13/09/2019.
//

#include <Console/Channels/NormalChannel.hpp>
#include <Console/NetUtils.hpp>

#include <fcntl.h>
#include <unistd.h>

void NormalChannel::AcceptConnection(SocketHandle handle, bool, Context* outContext) {

    // accept the client
    auto socket = accept(handle, nullptr, nullptr);
    if (socket == -1) {
        perror("accept evt()");
    }

    fcntl(socket, F_SETFL, O_NONBLOCK); // Mark socket as non blocking

    auto x =  Net::Utils::ContextFromSocketHandle(socket);

    outContext->socket = x.socket;
    outContext->secure = false;
}

void NormalChannel::DisposeConnection(Context* ctx) {

    close(ctx->socket.handle);

}

size_t NormalChannel::Read(Context* ctx, void *data, size_t dataLength) {

    return recv(ctx->socket.handle, data, dataLength, 0);

}

size_t NormalChannel::Write(Context* ctx, void *data, size_t dataLength) {

    return send(ctx->socket.handle, data, dataLength, 0);

}
