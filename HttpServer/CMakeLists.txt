# CMakeLists.txt for HttpServer
#
# © 2019 by Nuno Silva

cmake_minimum_required(VERSION 3.8)
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
        include/Http/HttpStatusCode.hpp)

set(Sources
        src/HttpServer.cpp src/HttpParser.cpp src/HttpHeader.cpp src/HttpRouter.cpp src/HttpRequest.cpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
    add_definitions(
            -DUNICODE -D_UNICODE
            -D_CRT_SECURE_NO_WARNINGS
            -DEXPORT_LIBRARY
    )
endif()

if(1)
    add_definitions(-DDEBUG_ENABLED)
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
