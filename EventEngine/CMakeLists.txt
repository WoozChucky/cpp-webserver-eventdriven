# CMakeLists.txt for EventEngine
#
# © 2019 by Nuno Silva

cmake_minimum_required(VERSION 3.13)
set(This EventEngine)

set(Headers
        include/Events/EventHandler.hpp
        include/Events/EventManager.hpp)

set(Sources )

if(APPLE)
    list(APPEND Sources
            src/MacOS/EventHandler.cpp
            src/MacOS/EventManager.cpp)
endif()

if(LINUX)
    list(APPEND Sources
            src/Linux/EventManager.cpp
            src/Linux/EventHandler.cpp)
endif()

if(MSVC)
    list(APPEND Sources
            src/Win/EventManager.cpp
            src/Win/EventHandler.cpp)

    add_definitions(
            -DUNICODE -D_UNICODE
            -D_CRT_SECURE_NO_WARNINGS
            -DEXPORT_LIBRARY
    )
endif()

include_directories(${LIBRESSL_INCLUDE_DIR})

add_library(${This} SHARED ${Sources} ${Headers})

find_package(LibreSSL REQUIRED)

target_include_directories(${This} PUBLIC include)

set_target_properties(${This} PROPERTIES
        FOLDER Libraries
        )

if(LINUX)
    message("This is Linux!")
    target_link_libraries(${This}
            pthread
            dl
            kqueue
            )
endif(LINUX)

target_link_libraries(${This} Abstractions ${OPENSSL_LIBRARIES})

# add_subdirectory(test)
