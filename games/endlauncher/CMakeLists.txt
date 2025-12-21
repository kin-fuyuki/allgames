cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wno-error")
file(GLOB_RECURSE ENDLAUNCHER "src/*.cpp")
add_executable(endlauncher ${ENDLAUNCHER})

set_target_properties(endlauncher PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/built/launcher/${PLATFORM_DIR}"
)
target_link_directories(
        endlauncher PUBLIC
        "${CMAKE_SOURCE_DIR}/link")
target_link_libraries(endlauncher PRIVATE
        "${CMAKE_SOURCE_DIR}/link/libenginend.so"
        "${CMAKE_SOURCE_DIR}/link/libdesktopraylib.so"
        curl zip
)
target_include_directories(endlauncher PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/lib/raylib/build/raylib/include
        ${CMAKE_SOURCE_DIR}/engine/include
)