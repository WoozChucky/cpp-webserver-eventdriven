# CMakeLists.txt for Abstractions
#
# © 2019 by Nuno Silva

cmake_minimum_required(VERSION 3.13)
set(This Abstractions)

set(Headers
        include/Abstractions/MemoryPool.hpp
        include/Abstractions/File.hpp
        include/Abstractions/Logger.hpp include/Abstractions/DynamicLibrary.hpp)

set(Sources
        src/MemoryPool.cpp
        src/File.cpp src/DynamicLibrary.cpp src/DynamicLibraryImpl.hpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
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

if(UNIX)
    target_link_libraries(${This}
            pthread
            dl
            )
endif(UNIX)

target_link_libraries(${This})

# add_subdirectory(test)
