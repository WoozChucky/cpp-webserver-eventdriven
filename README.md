# CPP WebServer EventDriven

## DISCLAIMER: Still a work in progress

### Introduction
...todo

The code is a tentative to implement one approach mentioned in this [article](http://www.kegel.com/c10k.html).
>  Serve many clients with each thread, and use nonblocking I/O and readiness change notification.

...todo

### Build Status

[![Linux x64](https://github.com/WoozChucky/cpp-webserver-eventdriven/actions/workflows/ccpp.yml/badge.svg)](https://github.com/WoozChucky/cpp-webserver-eventdriven/actions/workflows/ccpp.yml)

[![Build Status](https://travis-ci.com/WoozChucky/cpp-webserver-eventdriven.svg?branch=develop)](https://travis-ci.com/WoozChucky/cpp-webserver-eventdriven)

### Code Quality

[![CodeQL](https://github.com/WoozChucky/cpp-webserver-eventdriven/actions/workflows/codeql-analysis.yml/badge.svg?branch=develop)](https://github.com/WoozChucky/cpp-webserver-eventdriven/actions/workflows/codeql-analysis.yml)

[![Coverity](https://scan.coverity.com/projects/22856/badge.svg)](https://scan.coverity.com/projects/woozchucky-cpp-webserver-eventdriven)

### Modules

Very conceptual diagram of the project architecture

[![](https://mermaid.ink/img/eyJjb2RlIjoiY2xhc3NEaWFncmFtXG5cbiAgICBjbGFzcyBBYnN0cmFjdGlvbnMge1xuICAgICAgICBJT1xuICAgICAgICBNZW1vcnlcbiAgICAgICAgVGhyZWFkaW5nXG4gICAgICAgIERhdGEgVHlwZXNcbiAgICAgICAgU2hhcmVkTGlicmFyeSBMb2FkZXJcbiAgICAgICAgTGludXggU3VwcG9ydCgpXG4gICAgICAgIFdpbmRvd3MgU3VwcG9ydCgpXG4gICAgICAgIE1hY09TIFN1cHBvcnQoKVxuICAgIH1cblxuICAgIGNsYXNzIEV2ZW50RW5naW5lIHtcbiAgICAgICAgS2VybmVsIE5vdGlmaWNhdGlvbnNcbiAgICAgICAgRXZlbnQgTWFuYWdlcigpXG4gICAgICAgIEV2ZW50IEhhbmRsZXIoKVxuICAgIH1cblxuICAgIGNsYXNzIFNvY2tldFNlcnZlciB7XG4gICAgICAgIFRyYW5zcG9ydCBDaGFubmVsc1xuICAgICAgICBTb2NrZXQgRXZlbnRzXG4gICAgICAgIE5ldCBVdGlsaXRpZXMoKVxuICAgIH1cblxuICAgIGNsYXNzIFNvY2tldENsaWVudCB7XG4gICAgICAgIFRyYW5zcG9ydCBDaGFubmVsc1xuICAgICAgICBTb2NrZXQgRXZlbnRzXG4gICAgICAgIE5ldCBVdGlsaXRpZXMoKVxuICAgIH1cblxuICAgIGNsYXNzIEh0dHBTZXJ2ZXIge1xuICAgICAgICBIVFRQIFByb3RvY29sXG4gICAgICAgIEhUVFAgUGFyc2VyXG4gICAgICAgIEhUVFAgUm91dGVyXG4gICAgICAgIEhUVFAgQ29udGV4dCgpXG4gICAgfVxuXG4gICAgY2xhc3MgSHR0cENsaWVudCB7XG4gICAgICAgIEhUVFAgUHJvdG9jb2xcbiAgICAgICAgSFRUUCBQYXJzZXJcbiAgICAgICAgSFRUUCBSb3V0ZXNcbiAgICAgICAgSFRUUCBDb250ZXh0KClcbiAgICB9XG5cbiAgICBFdmVudEVuZ2luZSA8fC0tIEFic3RyYWN0aW9uc1xuICAgIFNvY2tldFNlcnZlciA8fC0tIEV2ZW50RW5naW5lXG4gICAgU29ja2V0U2VydmVyIDx8LS0gQWJzdHJhY3Rpb25zXG4gICAgSHR0cFNlcnZlciA8fC0tIFNvY2tldFNlcnZlclxuICAgIEh0dHBTZXJ2ZXIgPHwtLSBBYnN0cmFjdGlvbnNcblxuICAgIFNvY2tldENsaWVudCA8fC0tIEV2ZW50RW5naW5lXG4gICAgU29ja2V0Q2xpZW50IDx8LS0gQWJzdHJhY3Rpb25zXG4gICAgSHR0cENsaWVudCA8fC0tIFNvY2tldENsaWVudFxuICAgIEh0dHBDbGllbnQgPHwtLSBBYnN0cmFjdGlvbnNcbiAgICAgICAgICAgICIsIm1lcm1haWQiOnsidGhlbWUiOiJkZWZhdWx0In0sInVwZGF0ZUVkaXRvciI6ZmFsc2V9)](https://mermaid-js.github.io/mermaid-live-editor/#/edit/eyJjb2RlIjoiY2xhc3NEaWFncmFtXG5cbiAgICBjbGFzcyBBYnN0cmFjdGlvbnMge1xuICAgICAgICBJT1xuICAgICAgICBNZW1vcnlcbiAgICAgICAgVGhyZWFkaW5nXG4gICAgICAgIERhdGEgVHlwZXNcbiAgICAgICAgU2hhcmVkTGlicmFyeSBMb2FkZXJcbiAgICAgICAgTGludXggU3VwcG9ydCgpXG4gICAgICAgIFdpbmRvd3MgU3VwcG9ydCgpXG4gICAgICAgIE1hY09TIFN1cHBvcnQoKVxuICAgIH1cblxuICAgIGNsYXNzIEV2ZW50RW5naW5lIHtcbiAgICAgICAgS2VybmVsIE5vdGlmaWNhdGlvbnNcbiAgICAgICAgRXZlbnQgTWFuYWdlcigpXG4gICAgICAgIEV2ZW50IEhhbmRsZXIoKVxuICAgIH1cblxuICAgIGNsYXNzIFNvY2tldFNlcnZlciB7XG4gICAgICAgIFRyYW5zcG9ydCBDaGFubmVsc1xuICAgICAgICBTb2NrZXQgRXZlbnRzXG4gICAgICAgIE5ldCBVdGlsaXRpZXMoKVxuICAgIH1cblxuICAgIGNsYXNzIFNvY2tldENsaWVudCB7XG4gICAgICAgIFRyYW5zcG9ydCBDaGFubmVsc1xuICAgICAgICBTb2NrZXQgRXZlbnRzXG4gICAgICAgIE5ldCBVdGlsaXRpZXMoKVxuICAgIH1cblxuICAgIGNsYXNzIEh0dHBTZXJ2ZXIge1xuICAgICAgICBIVFRQIFByb3RvY29sXG4gICAgICAgIEhUVFAgUGFyc2VyXG4gICAgICAgIEhUVFAgUm91dGVyXG4gICAgICAgIEhUVFAgQ29udGV4dCgpXG4gICAgfVxuXG4gICAgY2xhc3MgSHR0cENsaWVudCB7XG4gICAgICAgIEhUVFAgUHJvdG9jb2xcbiAgICAgICAgSFRUUCBQYXJzZXJcbiAgICAgICAgSFRUUCBSb3V0ZXNcbiAgICAgICAgSFRUUCBDb250ZXh0KClcbiAgICB9XG5cbiAgICBFdmVudEVuZ2luZSA8fC0tIEFic3RyYWN0aW9uc1xuICAgIFNvY2tldFNlcnZlciA8fC0tIEV2ZW50RW5naW5lXG4gICAgU29ja2V0U2VydmVyIDx8LS0gQWJzdHJhY3Rpb25zXG4gICAgSHR0cFNlcnZlciA8fC0tIFNvY2tldFNlcnZlclxuICAgIEh0dHBTZXJ2ZXIgPHwtLSBBYnN0cmFjdGlvbnNcblxuICAgIFNvY2tldENsaWVudCA8fC0tIEV2ZW50RW5naW5lXG4gICAgU29ja2V0Q2xpZW50IDx8LS0gQWJzdHJhY3Rpb25zXG4gICAgSHR0cENsaWVudCA8fC0tIFNvY2tldENsaWVudFxuICAgIEh0dHBDbGllbnQgPHwtLSBBYnN0cmFjdGlvbnNcbiAgICAgICAgICAgICIsIm1lcm1haWQiOnsidGhlbWUiOiJkZWZhdWx0In0sInVwZGF0ZUVkaXRvciI6ZmFsc2V9)

> #### Abstractions
> - Responsible for most cross-platform support code.
>   - IO
>   - Threads
>   - Memory

> #### EventEngine
> - Responsible for implementing the an engine that listens and reacts to kernel IO notifications
>   - File descriptors
>   - Pipes
>   - Directories

> #### SocketServer
> - Responsible for operations of sockets that listen to incoming connections.

> #### HttpServer
> - Responsible for implementing the [HTTP Protocol](https://tools.ietf.org/html/rfc2616) (at least some of it...) with a SERVER role.

> #### SocketClient
> - Responsible for operations of sockets that connect to other sockets that are listening for connections.

> #### HttpClient
> - Responsible for implementing the [HTTP Protocol](https://tools.ietf.org/html/rfc2616) (at least some of it...) with a CLIENT role.

### Features
- Non Blocking Sockets (Server AND Clients)
- Event Engine to handle multiple concurrent connections using kernel notifications ([epoll](https://man7.org/linux/man-pages/man7/epoll.7.html), [kevent](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/kevent.2.html))
    - Non-blocking I/O operations
    - Edge-triggered readiness notifications
    - 1 Connection Accept Thread
    - 1 Optional(can be a blocking call) WebServer Listen Thread
- Built-in support for TLS using LibreSSL (allows for OpenSSL or any other openssl based fork.)
- Cross-platform support
    - Linux (usable)
    - Windows (work-in-progress)
    - MacOS (work-in-progress)

### Roadmap
- Improve Abstractions module
    - Remove _if defs_ and create separate implementation files
- Improve Cross-platform Support
    - Windows
      - __EventEngine__: There is no out-of-the-box kernel notifications solution
  - MacOS
      - __EventEngine__: Implement with kqueue (90% done)
- Improve general Memory Pool usage
- HTTP
    - Improve request parser algorithm
    - Improve response data structure
    - Add support for HTTP2
    - Add support for compression
        - Maybe zlib or GZIP ?
    - Add support for http uploads
    - Improve support for Connection header: KeepAlive and Close
    - Add support for Cache header
- Develop WebSocket module
  - With support for  TLS
- Improve TLS module to be a compilation option and not mandatory
- Improve socket message buffering
- Develop shutdown (fatal/gracefully) of sockets
- Develop the Client modules 
  - SocketClient
  - HttpClient
  - WebSocketClient
- Refactor Channel module to use C++ "interfaces - pure virtual methods" allowing other Transports to be used.
- Refactor TLS connect/accept to use callbacks for async operations
- Improve support to dynamically load plugins, such as new route handlers.  

### Http WebServer Code Example
```cpp

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

    http->Handle("/", HandleBaseRoute); // defaults to GET

    http->Handle("/register", HttpMethod::POST, HandleRegisterRoute);

    try {
        http->Boot(); // blocking
    } catch (std::exception& e) {
        TRACE("%s", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Response is written to address of *request.
void HandleBaseRoute(HttpRequest* request, HttpResponse* response) {
    // do stuff
    TRACE("%s", request->GetHeader("Content-Length").data());
    TRACE("%s", request->GetBody().data());
    
    response->SetStatusCode(HttpStatusCode::OK);
    response->SetProtocol(HttpProtocol::V1_1);
    response->SetBody("{\"obj\": true}");
    response->AddHeader(HttpHeader("Connection", "Keep-Alive"));
    response->AddHeader(HttpHeader("Content-Type", "application/json"));
}

void HandleRegisterRoute(HttpRequest* request, HttpResponse* response) {
    // do stuff
}

```
