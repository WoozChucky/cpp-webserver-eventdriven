//
// Created by Nuno Levezinho Silva on 13/09/2019.
//

#ifndef SECURECHANNEL_HPP
#define SECURECHANNEL_HPP

#include <Console/Channels/IChannel.hpp>
#include <tls.h>
#include <mutex>
#include <Console/Common/MemoryPool.hpp>

class SecureChannel : public IChannel {

public:

    explicit SecureChannel();

    void AcceptConnection(SocketHandle handle, Context* outContext) override;
    void DisposeConnection(Context* ctx) override;
    std::string Read(Context *ctx) override;
    size_t Write(Context* ctx, void* data, size_t dataLength) override;

private:
    TLS* _serverTls{};
    TLSConfig* _tlsConfig{};
    std::mutex _mutex;

    MemoryPool* memoryPool;

    const uint8_t *privateKey{};
    size_t privateKeySize{};

    const uint8_t *publicKey{};
    size_t publicKeySize{};

    std::vector<char> pub;
    std::vector<char> key;

    void LoadPrivateKey(const char* filename);
    void LoadPublicKey(const char* filename);
    void prepare();

};

#endif //SECURECHANNEL_HPP
