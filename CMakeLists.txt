cmake_minimum_required(VERSION 3.20)

if(WIN32)
    set(PLATFORM_DIR "windows")
else()
    set(PLATFORM_DIR "linux")
endif()

add_subdirectory(lib/raylib)
add_subdirectory(engine)
add_subdirectory(games/forespend)
include(ExternalProject)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/objs/${PLATFORM_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/${PLATFORM_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/built/runtime/${PLATFORM_DIR})