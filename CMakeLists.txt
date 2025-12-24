cmake_minimum_required(VERSION 4.2.0)

project(mutavive
    LANGUAGES
    CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_CXX_FLAGS "-Wall")

add_executable(${CMAKE_PROJECT_NAME}
    src/main.cpp
    src/components.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/dependencies/raylib-5.5_macos/include
    INTERFACE
)

add_library(raylib SHARED IMPORTED)

set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/dependencies/raylib-5.5_macos/lib/libraylib.5.5.0.dylib
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    raylib
)
