cmake_minimum_required(VERSION 3.20)
SET(CMAKE_CXX_FLAGS "-std=c++23 -Werror")
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
add_library(enginend SHARED ${ENGINE_SOURCES})
target_include_directories(enginend PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        
)

target_link_directories(
        enginend PUBLIC
        "${CMAKE_SOURCE_DIR}/link")
set(ENGINE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(ENGINE_EXPORT_DIR "${CMAKE_SOURCE_DIR}/include/enginend")
set_target_properties(enginend PROPERTIES
        CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/link"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/link"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/link"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/link"
)

file(GLOB_RECURSE HEADER_FILES "${ENGINE_SRC_DIR}/*.h" "${ENGINE_SRC_DIR}/*.hpp")
foreach(HEADER ${HEADER_FILES})
    file(RELATIVE_PATH REL_PATH "${ENGINE_SRC_DIR}" "${HEADER}")

    set(DEST "${ENGINE_EXPORT_DIR}/${REL_PATH}")

    get_filename_component(DEST_DIR "${DEST}" DIRECTORY)

    add_custom_command(
            TARGET enginend PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEST_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${HEADER}" "${DEST}"
    )
endforeach()