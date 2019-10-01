//
// Created by Nuno Levezinho Silva on 10/09/2019.
//

/**
The non-blocking tcp server can be implemented as follows:

1. Create a socket
2. Mark it as non-blocking (this will make even the accept call non-blocking)
3. Bind it to an address
4. Listen on the socket
5. Create an epoll instance
6. Add your socket to the epoll instance (this way the incoming requests can be monitored through event notification)
7. Create a read event queue
8. Create threads for processing tasks from read queue
9. Create a write event queue
10. Create threads for processing tasks in the write queue
11. Wait for events on epoll instance in a loop
12. For incoming requests events
    1. call accept
    2. mark the accepted socket as non-blocking
    3. add it to the epoll instance for monitoring
13. For read events, push the file descriptor and user data to read event queue
14. For write events, push the file descriptor and user data to write event queue
15. For close events, remove the file descriptor from the epoll instance
 */

#include <Http/HttpServer.hpp>
#include <Abstractions/Logger.hpp>
#include <Socket/SocketOptionsBuilder.hpp>

int main(int argc, char **argv) {

    TRACE("%s - %d", "a", 5);

    auto builder = new SocketOptionBuilder();
    auto options = builder
            ->WithReuseAddress()
            ->WithReusePort()
            ->WithKeepAlive()
            ->WithMaxQueuedConnection(100)
            ->WithServerPort(443)
            ->WithSSL(true)
            ->WithCertificate("cert.pem")
            ->WithPrivateKey("key.pem")
            ->Build();

    auto http = new HttpServer(options);

    http->Handle("/",
            [](HttpRequest* request, HttpResponse* response) -> void {
        // do stuff
        fprintf(stdout, "%s\n", request->GetHeader("Content-Length").GetValue().c_str());
        fprintf(stdout, "%s\n", request->GetBody().c_str());
    });

    http->Handle("/register",
            HttpMethod::POST,
            [](HttpRequest* request, HttpResponse* response) -> void {
        // do stuff
    });

    http->Boot();

    return 0;
}
