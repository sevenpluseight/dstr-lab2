cmake_minimum_required(VERSION 3.10)
project(DSTR_Hospital_System)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cross-platform Compiler Flags
if(MSVC)
    add_compile_options(/W4 /WX)  # Treat warnings as errors
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Core Library
add_library(core STATIC
        auth/src/login.cpp
        auth/src/session_manager.cpp
        supplies/src/add_supply.cpp
        supplies/src/data_handler.cpp
        supplies/src/medical_supply_manager.cpp
        supplies/src/removed_supply.cpp
        supplies/src/stack.cpp
        supplies/src/view_supply.cpp
        utils/path_utils.cpp
)

# Include directories for core library
target_include_directories(core PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/utils"
        "${CMAKE_CURRENT_SOURCE_DIR}/auth/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/ambulances/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/emergencies/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/patients/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/supplies/include"
)

# Main Executable
add_executable(main_app main.cpp)
target_link_libraries(main_app PRIVATE core)

# Auto-discover other *_main.cpp executables
file(GLOB MAIN_FILES "*_main.cpp")
foreach(MAIN_FILE ${MAIN_FILES})
    get_filename_component(EXE_NAME ${MAIN_FILE} NAME_WE)
    add_executable(${EXE_NAME} ${MAIN_FILE})
    target_link_libraries(${EXE_NAME} PRIVATE core)
endforeach()

# CSV Auto-copy (Rebuild on Change)
file(GLOB DATA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/data/*.csv")

# Ensure data folder exists in build directory
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/data")

set(CSV_TARGETS "")
foreach(DATA_FILE ${DATA_FILES})
    get_filename_component(FILE_NAME ${DATA_FILE} NAME)
    set(DEST_FILE "${CMAKE_CURRENT_BINARY_DIR}/data/${FILE_NAME}")

    add_custom_command(
            OUTPUT "${DEST_FILE}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DATA_FILE}" "${DEST_FILE}"
            DEPENDS "${DATA_FILE}"
            COMMENT "Copying ${FILE_NAME} to build/data..."
            VERBATIM
    )
    list(APPEND CSV_TARGETS "${DEST_FILE}")
endforeach()

# Group all CSV copy commands
add_custom_target(copy_csv ALL
        DEPENDS ${CSV_TARGETS}
        COMMENT "Copying all CSV files to build/data..."
)

# Make executables depend on CSV files
add_dependencies(main_app copy_csv)
add_dependencies(core copy_csv)

foreach(MAIN_FILE ${MAIN_FILES})
    get_filename_component(EXE_NAME ${MAIN_FILE} NAME_WE)
    add_dependencies(${EXE_NAME} copy_csv)
endforeach()
