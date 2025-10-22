cmake_minimum_required(VERSION 3.15)
project(lab_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform-independent warnings
if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Detect platform
if (APPLE)
    message(STATUS "Building on macOS")
elseif (WIN32)
    message(STATUS "Building on Windows")
elseif (UNIX)
    message(STATUS "Building on Linux")
endif()

# Add executable
add_executable(lab_2
    main.cpp
    config.h
)
