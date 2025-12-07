cmake_minimum_required(VERSION 4.2.0)

project(mutavive)

add_executable(${CMAKE_PROJECT_NAME}
    src/main.cpp
    src/components.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/dependencies/raylib-5.5_macos/include
    INTERFACE
)

set(RAYLIB_LOC ${CMAKE_SOURCE_DIR}/dependencies/raylib-5.5_macos/lib/libraylib.5.5.0.dylib)

add_library(raylib SHARED IMPORTED)

set_target_properties(raylib 
    PROPERTIES
    IMPORTED_LOCATION ${RAYLIB_LOC}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    raylib
)

set_property(TARGET
    ${CMAKE_PROJECT_NAME} 
    PROPERTY
    CXX_STANDARD
    20
)
