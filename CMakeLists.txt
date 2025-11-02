cmake_minimum_required(VERSION 3.10)
project(DSTR_Hospital_System)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_library(core STATIC
        auth/src/login.cpp
        auth/src/session_manager.cpp
        supplies/src/add_supply.cpp
        supplies/src/load_supply_data.cpp
        supplies/src/medical_supply_manager.cpp
        supplies/src/removed_supply.cpp
        supplies/src/stack.cpp
        supplies/src/view_supply.cpp
)

target_include_directories(core PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/utils"
        "${CMAKE_CURRENT_SOURCE_DIR}/auth/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/ambulances/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/emergencies/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/patients/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/supplies/include"
)

add_executable(main_app main.cpp)
target_link_libraries(main_app PRIVATE core)

file(GLOB main_files "*_main.cpp")
foreach(main_file ${main_files})
    string(REPLACE "_main.cpp" "" exe_name ${main_file})
    add_executable(${exe_name} ${main_file})
    target_link_libraries(${exe_name} PRIVATE core)
endforeach()

# Automatic CSV update
file(GLOB DATA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/data/*.csv")

# Create the data directory in the build folder
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/data")

set(CSV_DESTINATIONS "") # Initialize list

foreach(DATA_FILE ${DATA_FILES})
    get_filename_component(FILE_NAME ${DATA_FILE} NAME)
    set(DESTINATION_FILE "${CMAKE_CURRENT_BINARY_DIR}/data/${FILE_NAME}")

    add_custom_command(
            OUTPUT "${DESTINATION_FILE}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DATA_FILE}"
            "${DESTINATION_FILE}"
            DEPENDS "${DATA_FILE}"
            COMMENT "Copying ${FILE_NAME} to build/data"
    )
    list(APPEND CSV_DESTINATIONS "${DESTINATION_FILE}")
endforeach()

# Custom target to group all CSV copy commands
add_custom_target(copy_csv ALL
        DEPENDS ${CSV_DESTINATIONS}
        COMMENT "Copying CSV files to build directory..."
)

# Ensure main_app depends on CSV copy
add_dependencies(main_app copy_csv)
