//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef NETUTILS_HPP
#define NETUTILS_HPP

#include <Socket/Sugar.hpp>

namespace Net {

    class Utils {

    public:
        static void ParseIPAddress(SocketAddress* address, char* ip, S32 ipSize);
        static void ParseIPv4Address(SocketAddressIn* address, char* ip, S32 ipSize);
        static void ParseIPv6Address(SocketAddressIn6* address, char* ip, S32 ipSize);

        static IPType GetAddressFamily(SocketAddress* address);
        static void GetAddressFamily(SocketAddress* address, IPType* type);

        static SocketContext ContextFromSocketHandle(SocketHandle handle);

        static unsigned int GetPortFromAddress(SocketAddress *address);
        static void GetPortFromAddress(SocketAddress *address, U8& port);
    };

}

#endif //NETUTILS_HPP
