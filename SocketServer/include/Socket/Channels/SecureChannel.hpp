//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef SECURECHANNEL_HPP
#define SECURECHANNEL_HPP

#include <Socket/Channels/IChannel.hpp>
#include <Abstractions/MemoryPool.hpp>

class SecureChannel : public IChannel {

public:

    explicit SecureChannel(const std::string& certificatePath, const std::string& privateKeyPath);

    void AcceptConnection(SocketHandle handle, SocketContext* outContext) override;
    void DisposeConnection(SocketContext* ctx) override;
    std::string Read(SocketContext *ctx) override;
    size_t Write(SocketContext* ctx, Buffer* buffer) override;
    void Terminate() override;

private:
    TLS* _serverTls{};
    TLSConfig* _tlsConfig{};

    MemoryPool* memoryPool;

    const U8 *privateKey{};
    size_t privateKeySize{};

    const U8 *publicKey{};
    size_t publicKeySize{};

    std::vector<U8> pub;
    std::vector<U8> key;

    void LoadPrivateKey(const char* filename);
    void LoadPublicKey(const char* filename);
    void prepare();

};

#endif //SECURECHANNEL_HPP
