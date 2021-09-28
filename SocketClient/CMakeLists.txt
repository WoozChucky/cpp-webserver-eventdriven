# CMakeLists.txt for SocketServer
#
# © 2019 by Nuno Silva

cmake_minimum_required(VERSION 3.13)
set(This SocketClient)

set(Headers
        include/Socket/Client.hpp
        include/Socket/TlsDecorator.hpp)

set(Sources
        src/Client.cpp)

include_directories(${LIBRESSL_INCLUDE_DIR})

if(APPLE)

elseif(UNIX)
    # list(APPEND Sources
    #         src/Events/LinuxEventManager.cpp
    #         src/Events/LinuxEventHandler.cpp)
elseif(MSVC)

endif()

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

target_link_libraries(${This} LibreSSL::TLS Abstractions)

# add_subdirectory(test)
