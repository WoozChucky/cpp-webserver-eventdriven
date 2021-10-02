# CMakeLists.txt for WebServer
#
# © 2019 by Nuno Silva

cmake_minimum_required(VERSION 3.13)
set(This WebServer)

set(Headers )

set(Sources
        src/main.cpp)

if(MSVC)
    add_definitions(
            -DUNICODE -D_UNICODE
            -D_CRT_SECURE_NO_WARNINGS
            -DEXPORT_LIBRARY
    )
endif()

add_executable(${This} ${Sources} ${Headers})

set(LIBRESSL_ROOT_DIR "C:/Program Files (x86)/LibreSSL")
set(LIBRESSL_CRYPTO_LIBRARY "C:/Program Files (x86)/LibreSSL/lib/crypto-46.lib")
set(LIBRESSL_TLS_LIBRARY "C:/Program Files (x86)/LibreSSL/lib/tls-20.lib")
set(LIBRESSL_SSL_LIBRARY "C:/Program Files (x86)/LibreSSL/lib/ssl-48.lib")
#find_package(LibreSSL REQUIRED)

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

if(MSVC)
    target_link_libraries(${This} Abstractions SocketServer HttpServer tls ws2_32)
endif(MSVC)

target_link_libraries(${This} Abstractions SocketServer HttpServer tls)
