cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wno-error")
file(GLOB_RECURSE FORESPEND_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(forespend ${FORESPEND_SOURCES})

set_target_properties(forespend PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/built/forespend/${PLATFORM_DIR}/bin"
)

target_link_directories(
        forespend PUBLIC
        "${CMAKE_SOURCE_DIR}/link")
target_link_libraries(forespend PRIVATE
        "${CMAKE_SOURCE_DIR}/link/libenginend.so"
        "${CMAKE_SOURCE_DIR}/link/libdesktopraylib.so"
)
target_include_directories(forespend PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/lib/raylib/build/raylib/include
        ${CMAKE_SOURCE_DIR}/engine/include
)