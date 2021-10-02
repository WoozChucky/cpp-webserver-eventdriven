//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include "Socket/Channels/NormalChannel.hpp"

#include "Socket/NetUtils.hpp"
#include <fcntl.h>


NormalChannel::NormalChannel() {

	this->memoryPool = new MemoryPool(1024, 5, 100);
}

void NormalChannel::AcceptConnection(SocketHandle handle, SocketContext* outContext) {

    //TODO(Levezinho): Implement this

    // accept the client
    auto socket = accept(handle, nullptr, nullptr);
    if (socket == -1) {
        perror("accept evt()");
    }

    Net::Utils::SetNonBlocking(handle); // Mark socket as non blocking

    auto x =  Net::Utils::ContextFromSocketHandle(socket);

    outContext->Socket = x.Socket;
    outContext->Secure = false;
}

void NormalChannel::DisposeConnection(SocketContext* ctx) {
    Net::Utils::CloseSocket(ctx->Socket.Handle);
}

std::string NormalChannel::Read(SocketContext* ctx) {

	auto readStream = this->memoryPool->Allocate<char>();

	S32 totalReadBytes = 0;
	S32 lastReadBytes;

	std::string fullBufferedRequest;

	bool finishedReceivingData = false;

	while (!finishedReceivingData) {

		lastReadBytes = recv(ctx->Socket.Handle, readStream, this->memoryPool->BlockSize(), 0);
		
		if (lastReadBytes >= 0) {
			totalReadBytes += lastReadBytes;
			fullBufferedRequest.append(readStream);
		}

		memset(readStream, 0, this->memoryPool->BlockSize());

		if (lastReadBytes == -1) {
			finishedReceivingData = true;
		}
	}

	this->memoryPool->Release(readStream);

	TRACE("Read %d total bytes.", totalReadBytes);

	return fullBufferedRequest;
}

size_t NormalChannel::Write(SocketContext* ctx, Buffer* buffer) {

	auto sentBytes = send(ctx->Socket.Handle, reinterpret_cast<char *>(buffer->Data), buffer->Size, 0);

	free(buffer->Data);
	free(buffer);

	return sentBytes;
}

void NormalChannel::Terminate() {
    //TODO(Levezinho): Implement this
}
