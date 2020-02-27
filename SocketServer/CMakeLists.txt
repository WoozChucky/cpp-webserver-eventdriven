# CMakeLists.txt for SocketServer
#
# © 2019 by Nuno Silva

cmake_minimum_required(VERSION 3.8)
set(This SocketServer)

set(Headers
        include/Socket/Sugar.hpp
        include/Socket/Channels/IChannel.hpp
        include/Socket/Events/MacEventManager.hpp
        include/Socket/Events/EventHandler.hpp
        include/Socket/SocketOptionsBuilder.hpp
        include/Socket/Server.hpp)

set(Sources
        include/Socket/NetUtils.hpp
        src/NetUtils.cpp
        include/Socket/Channels/SecureChannel.hpp
        src/Channels/NormalChannel.cpp
        src/Channels/SecureChannel.cpp
        include/Socket/Channels/NormalChannel.hpp
        src/Server.cpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(/usr/local/Cellar/openssl@1.1/1.1.1c/include)
include_directories(${LIBRESSL_INCLUDE_DIR})

if(APPLE)
    list(APPEND Sources
            src/Events/MacEventManager.cpp)
elseif(UNIX)
    list(APPEND Sources
            src/Events/LinuxEventManager.cpp
            src/Events/LinuxEventHandler.cpp)
elseif(MSVC)

endif()

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

target_link_libraries(${This} LibreSSL::TLS Abstractions)

# add_subdirectory(test)