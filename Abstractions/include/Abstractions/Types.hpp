//
// Created by nunol on 04/03/2020.
//

#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>
#include <string>
#include <tls.h>

#include <sys/event.h>


using U8            = uint8_t;
using U16           = uint16_t;
using U32           = uint32_t;
using U64           = uint64_t;

using S8            = int8_t;
using S16           = int16_t;
using S32           = int32_t;
using S64           = int64_t;

using R32           = float;
using R64           = double;

using Memory        = void*;

using EventHandle   = S32;
using Event         = struct kevent;

using TLS           = struct tls;
using TLSConfig     = struct tls_config;
using SocketHandle  = S32;

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

enum EventAction {
    Add = 0x0001,
    Delete = 0x0002,
    Enable = 0x0004,
    Disable = 0x0008
};

enum EventType : S16 {
    Read        = (-1),
    Write       = (-2),
    Disconnect  = (-3),
    Weird       = (-10)
};

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
    // std::string Address;

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
    struct Socket Socket {0, 0, /*""*/ IPType::Unsupported, BlockingMode::Unknown };

    /**
     * @brief The tls context
     */
    tls* TLS{};

    /**-
     * @brief Flag indicating is this context is secure
     */
    bool Secure = false;

} SocketContext;

/**
 * @brief
 */
typedef struct HandledEvent {

    /**
     * @brief
     */
    SocketContext* Context;

    /**
     * @brief
     */
    EventType Type;

} HandledEvent;

/**
 * @brief
 */
typedef struct Buffer {

    /**
     * @brief
     */
    Memory Data {nullptr};

    /**
     * @brief
     */
    U16 Size {0};

} Buffer;

#endif //TYPES_HPP
