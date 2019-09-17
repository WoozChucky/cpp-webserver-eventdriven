//
// Created by Nuno Levezinho Silva on 10/09/2019.
//

#ifndef LCHAIN_CONSOLE_INCLUDE_CONSOLE_EXPORTS_H_
#define LCHAIN_CONSOLE_INCLUDE_CONSOLE_EXPORTS_H_

#include <netinet/in.h>
#include <sys/event.h>
#include <string>
#include <tls.h>
#include <vector>

typedef int SocketHandle;
typedef int EventHandle;
typedef struct kevent Event;
typedef struct sockaddr SocketAddress;
typedef struct sockaddr_in SocketAddressIn;
typedef struct sockaddr_in6 SocketAddressIn6;
typedef struct sockaddr_storage SocketStorage;
typedef struct tls TLS;
typedef struct tls_config TLSConfig;

enum IPType {
    Unsupported = 0x00,
    IPv4 = 0x04,
    IPv6 = 0x08
};

typedef struct {
    std::vector<int> SocketOptions;
    uint ServerPort;
    uint MaxQueuedConnections;
    bool SSLEnabled;
} ServerConfiguration ;

typedef struct Buffer {
    int size;
    char* data;

    Buffer(int size, char* data) : size(size), data(data) {}

} Buffer;

typedef struct {
    SocketHandle handle;
    unsigned int port;
    std::string address;
    IPType type;
} ClientSocket;

typedef struct Message {
    Buffer buffer;
    void* time{};
    void* rfu{};

    Message(int bytes, char* buffer) : buffer(bytes, buffer) {}

} Message;

typedef struct {
    ClientSocket socket {0, 0, nullptr, IPType::Unsupported};
    TLS* tls{nullptr};
    bool secure = false;
} Context;



#endif //LCHAIN_CONSOLE_INCLUDE_CONSOLE_EXPORTS_H_
