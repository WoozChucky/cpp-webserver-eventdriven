# CMakeLists.txt for WebClient
#
# © 2019 by Nuno Silva

cmake_minimum_required(VERSION 3.13)
set(This WebClient)

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

target_link_libraries(${This} Abstractions SocketServer HttpServer LibreSSL::TLS)
