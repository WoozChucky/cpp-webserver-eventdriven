# CMakeLists.txt for HttpServer
#
# © 2019 by Nuno Silva

cmake_minimum_required(VERSION 3.13)
set(This HttpServer)

set(Headers
        include/Http/Sugar.hpp
        include/Http/HttpServer.hpp
        include/Http/HttpRouter.hpp
        include/Http/HttpContext.hpp
        include/Http/HttpHeader.hpp
        include/Http/HttpMethod.hpp
        include/Http/HttpParser.hpp
        include/Http/HttpProtocol.hpp
        include/Http/HttpRequest.hpp
        include/Http/HttpResponse.hpp
        include/Http/HttpStatusCode.hpp
        include/Http/Exceptions/HttpException.hpp
        include/Http/Exceptions/NotFoundException.hpp
        )

set(Sources
        src/HttpServer.cpp src/HttpParser.cpp src/HttpHeader.cpp src/HttpRouter.cpp src/HttpRequest.cpp src/HttpResponse.cpp)

if(MSVC)
    add_definitions(
            -DUNICODE -D_UNICODE
            -D_CRT_SECURE_NO_WARNINGS
            -DEXPORT_LIBRARY
    )
endif()

add_library(${This} SHARED ${Sources} ${Headers})

find_package(LibreSSL REQUIRED)

target_include_directories(${This} PUBLIC include)

set_target_properties(${This} PROPERTIES
        FOLDER Libraries
        )

if(UNIX)
    target_link_libraries(${This}
            pthread
            dl
            ${OPENSSL_LIBRARIES}
            )
endif(UNIX)

target_link_libraries(${This} SocketServer LibreSSL::TLS Abstractions)

# add_subdirectory(test)
