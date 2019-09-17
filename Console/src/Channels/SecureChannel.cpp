//
// Created by Nuno Levezinho Silva on 13/09/2019.
//

#include <Console/Channels/SecureChannel.hpp>
#include <Console/Common/File.hpp>
#include <Console/NetUtils.hpp>
#include <fcntl.h>
#include <unistd.h>

void SecureChannel::prepare() {

    //std::lock_guard<std::mutex> lock{_mutex};

    unsigned int protocols = 0;
    std::string ciphers = "ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384";

    if (tls_init() < 0) {
        fprintf(stderr, "Error init\n");
    }

    _tlsConfig = tls_config_new();
    if(_tlsConfig == nullptr) {
        printf("tls_config_new error\n");
        exit(1);
    }

    _serverTls = tls_server();
    if(_serverTls == nullptr) {
        printf("tls_server error\n");
        exit(1);
    }

    if(tls_config_parse_protocols(&protocols, "secure") < 0) {
        printf("tls_config_parse_protocols error\n");
        exit(1);
    }

    tls_config_set_protocols(_tlsConfig, protocols);

    if(tls_config_set_ciphers(_tlsConfig, ciphers.c_str()) < 0) {
        printf("tls_config_set_ciphers error\n");
        exit(1);
    }

    if(tls_config_set_key_mem(_tlsConfig, this->privateKey, this->privateKeySize) < 0) {
        printf("tls_config_set_key_file error\n");
        exit(1);
    }

    if(tls_config_set_cert_mem(_tlsConfig, this->publicKey, this->publicKeySize) < 0) {
        printf("tls_config_set_cert_file error: %s\n", tls_error(_serverTls));
        exit(1);
    }

    if(tls_configure(_serverTls, _tlsConfig) < 0) {
        printf("tls_configure error: %s\n", tls_error(_serverTls));
        exit(1);
    }

}

SecureChannel::SecureChannel() {
    LoadPublicKey("cert.pem");
    LoadPrivateKey("key.pem");
}

void SecureChannel::AcceptConnection(SocketHandle handle, bool secureConnection, Context* outContext) {

    prepare();

    // accept the client
    auto socket = accept(handle, nullptr, nullptr);
    if (socket == -1) {
        perror("accept evt()");
    }

    fcntl(socket, F_SETFL, O_NONBLOCK); // Mark socket as non blocking

    if (tls_accept_socket(_serverTls, &outContext->tls, socket)) {
        fprintf(stdout, "tls_accept_socket error: %s\n", tls_error(_serverTls));
        exit(1);
    }

    auto ctx = Net::Utils::ContextFromSocketHandle(socket);

    outContext->socket = ctx.socket;
    outContext->secure = true;
}

void SecureChannel::DisposeConnection(Context* ctx) {

    std::lock_guard<std::mutex> lock{_mutex};

    fprintf(stderr, "Gonna dispose connection\n");

    auto ret = tls_close(ctx->tls);

    if (ret < 0) {
        fprintf(stdout, "tls_close error: %s\n", tls_error(ctx->tls));
    }

    tls_free(ctx->tls);
    close(ctx->socket.handle);
    fprintf(stderr, "Disposed connection\n");

    tls_close(this->_serverTls);
    tls_free(this->_serverTls);
    tls_config_free(this->_tlsConfig);

    ctx->tls = nullptr;
    this->_serverTls = nullptr;
    this->_tlsConfig = nullptr;
}

size_t SecureChannel::Read(Context* ctx, char **data, size_t dataLength) {

    auto totalReadBytes = 0;
    auto lastReadBytes = TLS_WANT_POLLIN;

    std::string fullBufferedRequest;

    char* readStream = static_cast<char *>(malloc(sizeof(char) * dataLength));

    while (lastReadBytes == TLS_WANT_POLLIN && totalReadBytes == 0) {

        lastReadBytes = tls_read(ctx->tls, readStream, dataLength);

        if (lastReadBytes >= 0)
            totalReadBytes += lastReadBytes;

        fullBufferedRequest.append(readStream);

        memset(readStream, 0, dataLength);
    }

    dataLength = (sizeof(char) * totalReadBytes) + 1;

    *data = static_cast<char *>(malloc(dataLength));

    memcpy(*data, fullBufferedRequest.data(), dataLength);

    free(readStream);

    return totalReadBytes;

}

size_t SecureChannel::Write(Context* ctx, void *data, size_t dataLength) {

    return tls_write(ctx->tls, data, dataLength);

}

void SecureChannel::LoadPrivateKey(const char *filename) {

    this->key = File::ReadAllBytes(filename);

    this->privateKey = reinterpret_cast<const uint8_t *>(key.data());
    this->privateKeySize = key.size();
}

void SecureChannel::LoadPublicKey(const char *filename) {

    this->pub = File::ReadAllBytes(filename);

    this->publicKey = reinterpret_cast<const uint8_t *>(pub.data());
    this->publicKeySize = pub.size();
}