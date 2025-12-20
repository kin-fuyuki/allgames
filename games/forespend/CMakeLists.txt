cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE FORESPEND_SOURCES "src/*.cpp")
add_executable(forespend ${FORESPEND_SOURCES})

set_target_properties(forespend PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/built/forespend/${PLATFORM_DIR}"
)
target_link_libraries(forespend PRIVATE engine raylib)
target_include_directories(engine PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/raylib/include
        ${CMAKE_SOURCE_DIR}/engine/include
)