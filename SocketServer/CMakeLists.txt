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
        src/Server.cpp)

if(APPLE)
    list(APPEND Sources
            src/Channels/MacOS/NormalChannel.cpp
            src/Channels/MacOS/SecureChannel.cpp
            src/MacOS/NetUtils.cpp)
elseif(LINUX)
    list(APPEND Sources
            src/Channels/Linux/NormalChannel.cpp
            src/Channels/Linux/SecureChannel.cpp
            src/Linux/NetUtils.cpp)
elseif(MSVC)
    list(APPEND Sources
            src/Channels/Win/NormalChannel.cpp
            src/Channels/Win/SecureChannel.cpp
            src/Win/NetUtils.cpp)

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

    add_definitions(
            -DUNICODE -D_UNICODE
            -D_CRT_SECURE_NO_WARNINGS
            -DEXPORT_LIBRARY
    )
endif()

add_library(${This} SHARED ${Sources} ${Headers})

target_include_directories(${This} PUBLIC include)

set_target_properties(${This} PROPERTIES
        FOLDER Libraries
        )

if(LINUX)
    target_link_libraries(${This}
            pthread
            dl
            )
endif(LINUX)

target_link_libraries(${This} tls Abstractions EventEngine)

# add_subdirectory(test)
