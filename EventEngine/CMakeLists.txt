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
    target_link_libraries(${This} pthread dl kqueue Abstractions)
endif(LINUX)

if(MSVC)
    target_link_libraries(${This} kqueue Abstractions)
endif(MSVC)

if(APPLE)
    target_link_libraries(${This} Abstractions)
endif(APPLE)

# add_subdirectory(test)
