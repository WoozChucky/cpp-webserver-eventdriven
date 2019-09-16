//
// Created by Nuno Levezinho Silva on 12/09/2019.
//

#ifndef NETUTILS_HPP
#define NETUTILS_HPP

#include <Console/exports.h>

namespace Net {

    class Utils {

    public:
        static void ParseIPAddress(SocketAddress* address, char* ip, int ipSize);
        static void ParseIPv4Address(SocketAddressIn* address, char* ip, int ipSize);
        static void ParseIPv6Address(SocketAddressIn6* address, char* ip, int ipSize);

        static IPType GetAddressFamily(SocketAddress* address);
        static void GetAddressFamily(SocketAddress* address, IPType* type);

        static Context ContextFromSocketHandle(SocketHandle handle);

        static unsigned int GetPortFromAddress(SocketAddress *address);
        static void GetPortFromAddress(SocketAddress *address, unsigned int& port);
    };

}

#endif //NETUTILS_HPP
