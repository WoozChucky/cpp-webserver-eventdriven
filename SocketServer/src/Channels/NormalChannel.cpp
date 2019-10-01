//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include "NormalChannel.hpp"

#include "../NetUtils.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

void NormalChannel::AcceptConnection(SocketHandle handle, SocketContext* outContext) {

    //TODO(Levezinho): Implement this

    // accept the client
    auto socket = accept(handle, nullptr, nullptr);
    if (socket == -1) {
        perror("accept evt()");
    }

    fcntl(socket, F_SETFL, O_NONBLOCK); // Mark socket as non blocking

    auto x =  Net::Utils::ContextFromSocketHandle(socket);

    outContext->Socket = x.Socket;
    outContext->Secure = false;
}

void NormalChannel::DisposeConnection(SocketContext* ctx) {

    //TODO(Levezinho): Implement this
    close(ctx->Socket.Handle);
}

std::string NormalChannel::Read(SocketContext* ctx) {

    //TODO(Levezinho): Implement this
    return reinterpret_cast<const char *>(recv(ctx->Socket.Handle, (void *) "", 0, 0));
}

size_t NormalChannel::Write(SocketContext* ctx, void *data, size_t dataLength) {
    //TODO(Levezinho): Implement this
    return 0;
}