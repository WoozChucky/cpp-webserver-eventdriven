# CMakeLists.txt for Abstractions
#
# © 2019 by Nuno Silva

cmake_minimum_required(VERSION 3.13)
set(This Abstractions)

set(Headers
        include/Abstractions/MemoryPool.hpp
        include/Abstractions/File.hpp
        include/Abstractions/Logger.hpp
        include/Abstractions/DynamicLibrary.hpp
        include/Abstractions/Types.hpp
        include/Abstractions/ThreadPool.hpp
        include/Abstractions/Format.hpp
        include/Abstractions/Directory.hpp
        include/Abstractions/Timer.hpp
        include/Abstractions/ConcurrentHashMap.hpp
        )

set(Sources
        src/MemoryPool.cpp
        src/File.cpp
        src/DynamicLibrary.cpp
        src/DynamicLibraryImpl.hpp
        src/Timer.cpp
        src/Linux/Directory.cpp)

if(MSVC)
    add_definitions(
            -DUNICODE -D_UNICODE
            -D_CRT_SECURE_NO_WARNINGS
            -DEXPORT_LIBRARY
    )
endif()

include_directories(../KQueue/include)

add_library(${This} SHARED ${Sources} ${Headers})

target_include_directories(${This} PUBLIC include)

set_target_properties(${This} PROPERTIES
        FOLDER Libraries
        )

if(UNIX)
    target_link_libraries(${This}
            pthread
            dl
            )
endif(UNIX)

target_link_libraries(${This} tls)

# add_subdirectory(test)
