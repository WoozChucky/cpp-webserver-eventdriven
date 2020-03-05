//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef SUGAR_HPP
#define SUGAR_HPP

#include <string>
#include <tls.h>
#include <vector>
#include <functional>

#include <Abstractions/Types.hpp>

using SocketAddress     = struct sockaddr;
using SocketAddressIn   = struct sockaddr_in;
using SocketAddressIn6  = struct sockaddr_in6;
using SocketStorage     = struct sockaddr_storage;
using TLS               = struct tls;
using TLSConfig         = struct tls_config;

#if LINUX
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>

using Event             = struct epoll_event;
#endif


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



using OnMessageDelegate = std::function<void(SocketContext* ctx, std::string& message)>;
using OnClientDelegate =  std::function<void(SocketContext* ctx)>;

#endif //SUGAR_HPP
