# CMakeLists.txt for LChain Project
#
# @ 2019 Nuno Levezinho

cmake_minimum_required(VERSION 3.13)
set(This LChain)
project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

enable_testing()

get_directory_property(ParentDirectory PARENT_DIRECTORY)
if(ParentDirectory STREQUAL "")
    # Define option for linking with the runtime statically.
    if(MSVC)
        option(RUNTIME_STATIC_LINKAGE "Link with runtime library statically" OFF)
    endif(MSVC)

    # If the runtime static linkage option is turned on, replace
    # compiler flag that selects runtime library variant.
    if(MSVC AND RUNTIME_STATIC_LINKAGE)
        foreach(flag
                CMAKE_C_FLAGS
                CMAKE_CXX_FLAGS
                CMAKE_C_FLAGS_DEBUG
                CMAKE_CXX_FLAGS_DEBUG
                CMAKE_C_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_RELEASE
                CMAKE_C_FLAGS_MINSIZEREL
                CMAKE_CXX_FLAGS_MINSIZEREL
                CMAKE_C_FLAGS_RELWITHDEBINFO
                CMAKE_CXX_FLAGS_RELWITHDEBINFO
                )
            if(${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "$(${flag})")
            endif(${flag} MATCHES "/MD")
        endforeach(flag)
    endif(MSVC AND RUNTIME_STATIC_LINKAGE)

    # Organize application, libraries, etc. in folder in the IDE.
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Set common output directory for all targets
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    # Options
    option(DEBUG_ENABLED "Enable DEBUG info to STDOUT" ON)
    if(DEBUG_ENABLED)
        add_definitions(-DDEBUG_ENABLED)
    endif()

    # Ensure proper loading of side-loaded shared libraries
    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    if(APPLE)
        set(CMAKE_INSTALL_RPATH "@loader_path")
    elseif(UNIX)
        set(CMAKE_INSTALL_RPATH "\$ORIGIN")
    endif()

    # Don't add the goofy "d" postfix for debug builds of libraries
    set(CMAKE_DEBUG_POSTFIX "")

    # Suppress Microsoft warnings about functions being
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)

    # Add modules path dir
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

    if(APPLE)
        add_definitions(-DMACOS)
    elseif(UNIX AND NOT APPLE)
        add_definitions(-DLINUX)
        set(LINUX TRUE)
    elseif(MSVC)
        add_definitions(-DWINDOWS)
    endif()

    # LibreSSL
    include(FindPkgConfig) # <-- include pkg-config needed by FindLibreSSL.cmake script
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake") # <-- use FindLibreSSL.cmake in /cmake folder

    set(LIBRESSL_APPS OFF)
    set(LIBRESSL_TESTS OFF)
    set(BUILD_SHARED_LIBS OFF)

    #find_package(LibreSSL 3.0.0 REQUIRED)

    message(CMake OS Platform: ${CMAKE_SYSTEM_NAME})

    if(NOT CMAKE_SYSTEM_NAME STREQUAL Darwin)
        add_subdirectory(KQueue) #Kqueue is already built in MacOS by default.
        include_directories(KQueue/include)
    endif()

    add_subdirectory(LibreSSL)
    include_directories(LibreSSL/include)

    # All other modules can be pulled in without further configuration.
    add_subdirectory(Abstractions)
    add_subdirectory(EventEngine)
    add_subdirectory(SocketServer)
    add_subdirectory(SocketClient)
    add_subdirectory(HttpServer)

    # Sample modules to aid development workflow
    add_subdirectory(Samples/WebServer)
    add_subdirectory(Samples/WebClient)

endif(ParentDirectory STREQUAL "")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
