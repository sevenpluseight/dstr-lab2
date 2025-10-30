cmake_minimum_required(VERSION 3.10)
project(DSTR_Hospital_System)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cross-Platform Compiler Settings
if(MSVC)
    # Microsoft Visual C++ Compiler
    add_compile_options(/W4 /WX) # High warning level, treat warnings as errors
else()
    # GCC/Clang
    add_compile_options(-Wall -Wextra -Wpedantic -Werror) # Common warnings
endif()

# Include Directories
# Add all component include directories and utils for direct header access
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/utils"
    "${CMAKE_CURRENT_SOURCE_DIR}/ambulances/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/emergencies/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/patients/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/supplies/include"
)

# Source File Discovery
# Automatically find all source files in the component directories
file(GLOB_RECURSE CORE_SOURCES
    "ambulances/src/*.cpp"
    "emergencies/src/*.cpp"
    "patients/src/*.cpp"
    "supplies/src/*.cpp"
)

# Library Creation (Conditional)
# Create a static library from all core components only if source files are found.
if(CORE_SOURCES)
    add_library(core STATIC ${CORE_SOURCES})
endif()

# Main Executable
# Create the main executable from main.cpp
add_executable(main_app main.cpp)

# Link the main executable against the core library if it exists
if(TARGET core)
    target_link_libraries(main_app PRIVATE core)
endif()

# Automatic Executable Discovery (for other mains)
# This section finds all files ending in "_main.cpp" and creates an executable for each.
file(GLOB main_files "*_main.cpp")

foreach(main_file ${main_files})
    # Get the name of the file without the extension to use as the executable name
    string(REPLACE "_main.cpp" "" exe_name ${main_file})
    add_executable(${exe_name} ${main_file})

    # Link against the core library if it exists
    if(TARGET core)
        target_link_libraries(${exe_name} PRIVATE core)
    endif()
endforeach()
