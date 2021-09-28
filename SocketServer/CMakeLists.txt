# CMakeLists.txt for SocketServer
#
# © 2019 by Nuno Silva

cmake_minimum_required(VERSION 3.13)
set(This SocketServer)

set(Headers
        include/Socket/Sugar.hpp
        include/Socket/NetUtils.hpp
        include/Socket/Channels/IChannel.hpp
        include/Socket/Channels/SecureChannel.hpp
        include/Socket/Channels/NormalChannel.hpp
        include/Socket/SocketOptionsBuilder.hpp
        include/Socket/Server.hpp)

set(Sources
        src/NetUtils.cpp
        src/Channels/NormalChannel.cpp
        src/Channels/SecureChannel.cpp
        src/Server.cpp)

include_directories(${LIBRESSL_INCLUDE_DIR})

if(MSVC)
    add_definitions(
            -DUNICODE -D_UNICODE
            -D_CRT_SECURE_NO_WARNINGS
            -DEXPORT_LIBRARY
    )
endif()

# Options
option(DEBUG_ENABLED "Enable DEBUG info to STDOUT" ON)
if(DEBUG_ENABLED)
    add_definitions(-DDEBUG_ENABLED)
endif()

add_library(${This} SHARED ${Sources} ${Headers})

find_package(LibreSSL REQUIRED)

target_include_directories(${This} PUBLIC include)

set_target_properties(${This} PROPERTIES
        FOLDER Libraries
        )

if(LINUX)
    target_link_libraries(${This}
            pthread
            dl
            ${OPENSSL_LIBRARIES}
            )
endif(LINUX)

target_link_libraries(${This} LibreSSL::TLS Abstractions EventEngine)

# add_subdirectory(test)
