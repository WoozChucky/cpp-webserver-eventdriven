# CMakeLists.txt for LChain Project
#
# @ 2019 Nuno Levezinho

cmake_minimum_required(VERSION 3.8)
set(This LChain)
project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

    # Keep Google Test's linkage of the runtime in sync with our own
    if (RUNTIME_STATIC_LINKAGE)
        set(gtest_force_shared_crt OFF CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as a statsic lib." FORCE)
    else(RUNTIME_STATIC_LINKAGE)
        set(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as a statsic lib." FORCE)
    endif(RUNTIME_STATIC_LINKAGE)

    # Organize application, libraries, etc. in folder in the IDE.
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
    elseif(UNIX)
        add_definitions(-DLINUX)
    elseif(MSVC)
        add_definitions(-DWINDOWS)
    endif()

    # Google Test
    set(BUILD_GTEST ON CACHE BOOL "Builds the googletest subproject" FORCE)
    set(BUILD_GMOCK ON CACHE BOOL "Builds the googlemock subproject" FORCE)
    add_subdirectory(googletest)
    set_target_properties(gtest gtest_main PROPERTIES
            FOLDER Libraries
            )

    # All other libraries can be pulled in without further configuration.
    add_subdirectory(Console)
    add_subdirectory(Abstractions)
    add_subdirectory(SocketServer)
    add_subdirectory(HttpServer)

endif(ParentDirectory STREQUAL "")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
