//
// Created by Nuno Levezinho Silva on 13/09/2019.
//

#include <Console/Channels/SecureChannel.hpp>
#include <Console/NetUtils.hpp>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <fcntl.h>
#include <unistd.h>

void SecureChannel::AcceptConnection(SocketHandle handle, bool secureConnection, Context* outContext) {

    // accept the client
    auto socket = accept(handle, nullptr, nullptr);
    if (socket == -1) {
        perror("accept evt()");
    }

    fcntl(socket, F_SETFL, O_NONBLOCK); // Mark socket as non blocking

    SSLContext ssl = SSLContext();

    auto ctx = Net::Utils::ContextFromSocketHandle(socket);

    outContext->socket = ctx.socket;

    if (secureConnection) {
        InitializeSLLContext(socket, &ssl);
        ctx.ssl = ssl;
        ctx.secure = true;

        outContext->ssl = ctx.ssl;
        outContext->secure = ctx.secure;
    }
}

void SecureChannel::DisposeConnection(Context* ctx) {

    close(ctx->socket.handle);
    SSL_free(ctx->ssl.ptr);
    SSL_CTX_free(ctx->ssl.ctx);

}

size_t SecureChannel::Read(Context* ctx, void *data, size_t dataLength) {

    auto totalReadBytes = 0;
    auto lastReadBytes = 1;

    std::string fullBufferedRequest;

    bool firstInteraction = true;

    char* readStream = static_cast<char *>(malloc(sizeof(char) * 20));

    while (lastReadBytes > 0) {

        lastReadBytes = SSL_read(ctx->ssl.ptr, readStream, 20);

        if (firstInteraction) {
            firstInteraction = false;

        }

        totalReadBytes += lastReadBytes;

        fullBufferedRequest.append(readStream);

        memset(readStream, 0, 20);
    }

    dataLength = sizeof(char) * totalReadBytes;

    data = malloc(dataLength);

    memcpy(data, fullBufferedRequest.data(), dataLength);

    free(readStream);

    return totalReadBytes;

}

size_t SecureChannel::Write(Context* ctx, void *data, size_t dataLength) {

    return SSL_write(ctx->ssl.ptr, data, dataLength);

}

void SecureChannel::InitializeSLLContext(SocketHandle  handle, SSLContext* pContext) {

    SSL_CTX             *sslCtx;
    SSL                 *ssl;
    const SSL_METHOD    *meth;

    meth = TLS_server_method();
    sslCtx = SSL_CTX_new(meth);

    if (!sslCtx) {
        ERR_print_errors_fp(stderr);
        pContext = nullptr;
    }

    if (SSL_CTX_use_certificate_file(sslCtx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(3);
    }

    if (SSL_CTX_use_RSAPrivateKey_file(sslCtx, "key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(4);
    }

    if (!SSL_CTX_check_private_key(sslCtx)) {
        fprintf(stderr,"Private key does not match the certificate public key\n");
        exit(5);
    }

    ssl = SSL_new(sslCtx);
    SSL_set_fd(ssl, handle);
    auto resp = SSL_accept(ssl);

    auto error = SSL_get_error(ssl, resp);

    int times = 0;

    if (error == 2) {
        while (resp != 1) {
            resp = SSL_accept(ssl);
            times++;
        }
        fprintf(stdout, "Connection was accepted in client certificate after %d tries.\n", times);
    }

    ERR_print_errors_fp(stderr);

    pContext->ptr = ssl;
    pContext->ctx = sslCtx;
}

