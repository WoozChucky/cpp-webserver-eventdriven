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
#include <Socket/SocketOptionsBuilder.hpp>

void HandleBaseRoute(HttpRequest* request, HttpResponse* response) {
    // do stuff
    if (request->HasBody()) {
        TRACE("%s", request->GetHeader("Content-Length").data())
        TRACE("%s", request->GetBody().data())
    }

    response->SetStatusCode(HttpStatusCode::OK);
    response->SetProtocol(HttpProtocol::V1_1);
    response->SetBody("{\"obj\": true}");
    response->AddHeader(HttpHeader("Connection", "Keep-Alive"));
    response->AddHeader(HttpHeader("Content-Type", "application/json"));
}

void HandleRegisterRoute(HttpRequest* request, HttpResponse* response) {
    // do stuff
}

int main(int argc, char **argv) {

    auto options = (new SocketOptionBuilder())
            ->WithReuseAddress()
            ->WithReusePort()
            ->WithKeepAlive()
            ->WithMaxQueuedConnection(100)
            ->WithServerPort(21000)
            ->WithSSL(true)
            ->WithCertificate("cert.pem")
            ->WithPrivateKey("key.pem")
            ->Build();

    auto http = new HttpServer(options);

    http->Handle("/", HttpMethod::GET, HandleBaseRoute);
    //http->Handle("/", HttpMethod::POST, HandleBaseRoute);

    http->Handle("/register",HttpMethod::POST, HandleRegisterRoute);

    auto pool = new ThreadPool(1);

    try {
        pool->enqueue([http]() -> void {
            //http->Boot();
        });
        http->Boot();
    } catch (std::exception& e) {
        TRACE("%s", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
