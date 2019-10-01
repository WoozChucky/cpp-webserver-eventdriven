//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef SUGAR_HPP
#define SUGAR_HPP

#include <string>
#include <tls.h>
#include <vector>

using U8    = uint8_t;
using U16   = uint16_t;
using U32   = uint32_t;
using U64   = uint64_t;

using S8    = int8_t;
using S16   = int16_t;
using S32   = int32_t;
using S64   = int64_t;

using R32 = float;
using R64 = double;

using Memory = void*;

using SocketHandle      = S32;
using EventHandle       = S32;
using Event             = struct kevent;
using SocketAddress     = struct sockaddr;
using SocketAddressIn   = struct sockaddr_in;
using SocketAddressIn6  = struct sockaddr_in6;
using SocketStorage     = struct sockaddr_storage;
using TLS               = struct tls;
using TLSConfig         = struct tls_config;

enum IPType {
    Unsupported = 0x00,
    IPv4 = 0x04,
    IPv6 = 0x08
};

enum BlockingMode {
    Unknown = 0x00,
    Blocking = 0x04,
    NonBlocking = 0x08
};

/**
 * @brief The SocketServer configuration to be passed on initialization.
 */
typedef struct ServerConfiguration {
    /**
     * @brief OS-specific server socket options. (i.e. Reuse Address, Reuse Port, etc..)
     */
    std::vector<S32> SocketOptions;

    /**
     * @brief The port where the SocketServer is listening at.
     */
    U16 ServerPort;

    /**
     * @brief This sets the number of connections that the SocketServer can have waiting
     * for him to accept them before automatically dropping new incoming connections.
     */
    U16 MaxQueuedConnections;

    /**
     * @brief This sets the requirement usage of SSL by the TCP SocketServer.
     *
     * @note If this is set to true, 'SSLCertificatePath' and 'SSLPrivateKeyPath' need to be defined.
     */
    bool SSLEnabled;

    /**
     * @brief The relative or full path to the file containing the ssl certificate.
     *
     * @note Only needed if 'SSLEnabled' is set to true.
     */
    std::string SSLCertificatePath;

    /**
     * @brief The relative or full path to the file containing the ssl private key.
     *
     * @note Only needed if 'SSLEnabled' is set to true.
     */
    std::string SSLPrivateKeyPath;

} ServerConfiguration ;

/**
 * @brief The Socket representation used in the Channel Layer.
 */
typedef struct Socket {
    /**
     * @brief The os-specific internal handle for the socket.
     */
    SocketHandle Handle;

    /**
     * @brief The port when the socket is connecting/listening from.
     *
     */
    U16 Port;

    /**
     * @brief The internet protocol address in a human-readable format.
     */
    std::string Address;

    /**
     * @brief Specifies the protocol used (IPv4 or IPv6).
     */
    IPType Type;

    /**
     * @brief Specifies the socket blocking mode.
     */
    BlockingMode Mode;

} Socket;

/**
 * @brief The context passed within the TCP SocketServer layer
 */
typedef struct SocketContext {

    /**
     * @brief The socket object
     */
    Socket Socket
            {0, 0, nullptr, IPType::Unsupported, BlockingMode::Unknown};

    /**
     * @brief The tls context
     */
    TLS* TLS{nullptr};

    /**
     * @brief Flag indicating is this context is secure
     */
    bool Secure = false;

} SocketContext;

#endif //SUGAR_HPP
