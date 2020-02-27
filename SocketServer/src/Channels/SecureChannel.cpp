//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include "Socket/Channels/SecureChannel.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <Abstractions/File.hpp>
#include <Abstractions/Logger.hpp>
#include <Socket/NetUtils.hpp>
#include <cstring>

void SecureChannel::prepare() {

    unsigned int protocols = 0;
    std::string ciphers = "ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384";

    if (tls_init() < 0) {
        TRACE("%s", "Error init");
    }

    _tlsConfig = tls_config_new();
    if(_tlsConfig == nullptr) {
        TRACE("%s", "tls_config_new error");
        exit(1);
    }

    _serverTls = tls_server();

    if(_serverTls == nullptr) {
        TRACE("%s", "tls_server error");
        exit(1);
    }

    if(tls_config_parse_protocols(&protocols, "secure") < 0) {
        TRACE("%s", "tls_config_parser_protocols error");
        exit(1);
    }

    tls_config_set_protocols(_tlsConfig, protocols);

    if(tls_config_set_ciphers(_tlsConfig, ciphers.c_str()) < 0) {
        TRACE("%s", "tls_config_set_ciphers error");
        exit(1);
    }

    if(tls_config_set_key_mem(_tlsConfig, this->privateKey, this->privateKeySize) < 0) {
        TRACE("%s", "tls_config_set_key_file error");
        exit(1);
    }

    if(tls_config_set_cert_mem(_tlsConfig, this->publicKey, this->publicKeySize) < 0) {
        TRACE("%s: %s", "tls_config_set_cert_file error", tls_error(_serverTls));
        exit(1);
    }

    if(tls_configure(_serverTls, _tlsConfig) < 0) {
        TRACE("%s: %s", "tls_configure error", tls_error(_serverTls));
        exit(1);
    }

}

SecureChannel::SecureChannel(const std::string& certificatePath, const std::string& privateKeyPath) {
    this->memoryPool = new MemoryPool(4096, 5, 100);

    try {

        LoadPublicKey(certificatePath.c_str());
        LoadPrivateKey(privateKeyPath.c_str());

    } catch (const std::exception& e) {
        TRACE("%s", e.what());
        throw;
    }
}

void SecureChannel::AcceptConnection(SocketHandle handle, SocketContext* outContext) {

    this->prepare();

    // accept the client
    auto socket = accept(handle, nullptr, nullptr);
    if (socket == -1) {
        perror("accept evt()");
    }

    fcntl(socket, F_SETFL, O_NONBLOCK); // Mark socket as non blocking

    if (tls_accept_socket(_serverTls, &outContext->TLS, socket) != 0) {
        TRACE("%s: %s", "tls_accept_socket error", tls_error(_serverTls));
        exit(1);
    }

    tls_handshake(outContext->TLS);

    auto ctx = Net::Utils::ContextFromSocketHandle(socket);

    outContext->Socket = ctx.Socket;
    outContext->Secure = true;
}

void SecureChannel::DisposeConnection(SocketContext* ctx) {

    // std::lock_guard<std::mutex> lock{_mutex};

    auto ret = tls_close(ctx->TLS);

    if (ret < 0) {
        TRACE("%s: %s", "tls_close error", tls_error(ctx->TLS));
    }

    tls_free(ctx->TLS);
    close(ctx->Socket.Handle);

    tls_close(this->_serverTls);
    tls_free(this->_serverTls);
    tls_config_free(this->_tlsConfig);

    ctx->TLS = nullptr;
    this->_serverTls = nullptr;
    this->_tlsConfig = nullptr;
}

std::string SecureChannel::Read(SocketContext *ctx) {

    auto totalReadBytes = 0;
    auto lastReadBytes = TLS_WANT_POLLIN;

    std::string fullBufferedRequest;

    auto readStream = this->memoryPool->Allocate<char>();

    while (lastReadBytes == TLS_WANT_POLLIN && totalReadBytes == 0) {

        lastReadBytes = tls_read(ctx->TLS, readStream, this->memoryPool->BlockSize());

        if (lastReadBytes >= 0)
            totalReadBytes += lastReadBytes;

        fullBufferedRequest.append(readStream);

        memset(readStream, 0, this->memoryPool->BlockSize());
    }

    this->memoryPool->Release(readStream);

    return fullBufferedRequest;
}

size_t SecureChannel::Write(SocketContext* ctx, Memory data, size_t dataLength) {

    return tls_write(ctx->TLS, data, dataLength);

}

void SecureChannel::LoadPrivateKey(const char *filename) {

    if (!File::Exists(filename)) {
        throw std::runtime_error("Failed to find file with PrivateKey.");
    }

    this->key = File::ReadAllBytes(filename);

    this->privateKey = reinterpret_cast<const U8 *>(key.data());
    this->privateKeySize = key.size();
}

void SecureChannel::LoadPublicKey(const char *filename) {

    if (!File::Exists(filename)) {
        throw std::runtime_error("Failed to find file with PublicKey.");
    }

    this->pub = File::ReadAllBytes(filename);

    this->publicKey = reinterpret_cast<const U8 *>(pub.data());
    this->publicKeySize = pub.size();
}