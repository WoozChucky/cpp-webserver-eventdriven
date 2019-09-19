# CMakeLists.txt for APIVivaServer
#
# © 2019 by Nuno Silva

cmake_minimum_required(VERSION 3.8)
set(This Console)

set(Headers
        include/Console/exports.h
        )

set(Sources
        src/main.cpp
        include/Console/SocketOptionBuilder.hpp include/Console/Server.hpp src/Server.cpp include/Console/EventManager.hpp src/EventManager.cpp include/Console/NetUtils.hpp src/NetUtils.cpp include/Console/Channels/IChannel.hpp include/Console/Channels/SecureChannel.hpp include/Console/Channels/NormalChannel.hpp src/Channels/SecureChannel.cpp src/Channels/NormalChannel.cpp include/Console/Common/MemoryPool.hpp src/Common/MemoryPool.cpp include/Console/HttpServer.hpp src/HttpServer.cpp include/Console/Common/File.hpp src/Common/File.cpp include/Console/Http/HttpRequest.hpp include/Console/Http/HttpResponse.hpp include/Console/Http/HttpStatusCode.hpp include/Console/Http/HttpMethod.hpp include/Console/Http/HttpRouter.hpp src/Http/HttpRouter.cpp include/Console/Http/HttpParser.hpp src/Http/HttpParser.cpp include/Console/Http/HttpHeader.hpp include/Console/Http/HttpProtocol.hpp src/Http/HttpRequest.cpp src/Http/HttpHeader.cpp include/Console/Http/HttpContext.hpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(../Core/include)
include_directories(/usr/local/Cellar/openssl@1.1/1.1.1c/include)
include_directories(${LIBRESSL_INCLUDE_DIR})

find_library(LIB_CORE LChainCore PATHS ../bin/lib)
set(SSL_LIBRA /usr/local/Cellar/openssl@1.1/1.1.1c/lib/libssl.dylib)
set(CRYPTO_LIBRA /usr/local/Cellar/openssl@1.1/1.1.1c/lib/libcrypto.dylib)

if(MSVC)
    add_definitions(
            -DUNICODE -D_UNICODE
            -D_CRT_SECURE_NO_WARNINGS
            -DEXPORT_LIBRARY
    )
endif()

if(${Debug} STREQUAL 1)
    add_definitions(-DDEBUG_ENABLED)
endif()

add_executable(${This} ${Sources} ${Headers})

find_package(LibreSSL REQUIRED)

target_include_directories(${This} PUBLIC include)

set_target_properties(${This} PROPERTIES
        FOLDER Applications
        )

if(UNIX)
    target_link_libraries(${This}
            pthread
            dl
            ${OPENSSL_LIBRARIES}
            )
endif(UNIX)

message(STATUS "${LIBRESSL_INCLUDE_DIR}")
message(STATUS "${SSL_LIBRA}")
message(STATUS "${CRYPTO_LIBRA}")

target_link_libraries(${This} ${LIB_CORE} ${SSL_LIBRA} ${CRYPTO_LIBRA} LibreSSL::TLS)

# add_subdirectory(test)
