//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Socket/NetUtils.hpp>

#include <Socket/Sugar.hpp>
#include <arpa/inet.h>

void Net::Utils::ParseIPv4Address(SocketAddressIn *address, char* ip, S32 ipSize) {

    inet_ntop(address->sin_family, &address->sin_addr, ip, ipSize);

}

void Net::Utils::ParseIPv6Address(SocketAddressIn6 *address, char *ip, S32 ipSize) {

    inet_ntop(address->sin6_family, &address->sin6_addr, ip, ipSize);

}

void Net::Utils::ParseIPAddress(SocketAddress *address, char *ip, S32 ipSize) {

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

SocketContext Net::Utils::ContextFromSocketHandle(SocketHandle handle) {

    SocketAddress address;
    U32 len = sizeof(address);
    char clientIp[60];
    U8 port = 0;
    IPType familyType;

    getsockname(handle, &address, &len);

    Net::Utils::ParseIPAddress(&address, clientIp, sizeof(clientIp));

    Net::Utils::GetPortFromAddress(&address, port);

    Net::Utils::GetAddressFamily(&address, &familyType);

    return SocketContext {
            {
                    handle,
                    port,
                    /*clientIp,*/
                    familyType,
                    NonBlocking
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

void Net::Utils::GetPortFromAddress(SocketAddress *address, U8& port) {
    port = GetPortFromAddress(address);
}
