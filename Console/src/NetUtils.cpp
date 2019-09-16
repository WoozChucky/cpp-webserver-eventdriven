//
// Created by Nuno Levezinho Silva on 12/09/2019.
//

#include <Console/NetUtils.hpp>
#include <arpa/inet.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"


void Net::Utils::ParseIPv4Address(SocketAddressIn *address, char* ip, int ipSize) {

    inet_ntop(address->sin_family, &address->sin_addr, ip, ipSize);

}

void Net::Utils::ParseIPv6Address(SocketAddressIn6 *address, char *ip, int ipSize) {

    inet_ntop(address->sin6_family, &address->sin6_addr, ip, ipSize);

}

void Net::Utils::ParseIPAddress(SocketAddress *address, char *ip, int ipSize) {

    switch (address->sa_family) {
        case AF_INET:
            ParseIPv4Address((SocketAddressIn *)address, ip, ipSize);
            break;
        case AF_INET6:
            ParseIPv6Address((SocketAddressIn6 *)address, ip, ipSize);
            break;
        default:
            break;
    }
}

IPType Net::Utils::GetAddressFamily(SocketAddress *address) {
    switch (address->sa_family) {
        case AF_INET:
            return IPv4;
        case AF_INET6:
            return IPv6;
        default:
            return Unsupported;
    }
}

void Net::Utils::GetAddressFamily(SocketAddress *address, IPType* type) {
    *type = GetAddressFamily(address);
}

Context Net::Utils::ContextFromSocketHandle(SocketHandle handle) {

    SocketAddress address;
    socklen_t len = sizeof(address);
    char clientIp[60];
    unsigned int port = 0;
    IPType familyType;

    getsockname(handle, &address, &len);

    Net::Utils::ParseIPAddress(&address, clientIp, sizeof(clientIp));

    Net::Utils::GetPortFromAddress(&address, port);

    Net::Utils::GetAddressFamily(&address, &familyType);

    return Context {
        {
            handle,
            port,
            clientIp,
            familyType
        }
    };
}

unsigned int Net::Utils::GetPortFromAddress(SocketAddress *address) {

    switch (address->sa_family) {
        case AF_INET:
            return ((SocketAddressIn *)address)->sin_port;
        case AF_INET6:
            return ((SocketAddressIn6 *)address)->sin6_port;
        default:
            return 0;
    }
}

void Net::Utils::GetPortFromAddress(SocketAddress *address, unsigned int& port) {
    port = GetPortFromAddress(address);
}

#pragma clang diagnostic pop