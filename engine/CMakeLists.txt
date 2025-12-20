cmake_minimum_required(VERSION 3.20)
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
add_library(engine STATIC ${ENGINE_SOURCES})
target_include_directories(engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/raylib/include
        
)

set(ENGINE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(ENGINE_EXPORT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(GLOB_RECURSE HEADER_FILES "${ENGINE_SRC_DIR}/*.h" "${ENGINE_SRC_DIR}/*.hpp")
foreach(HEADER ${HEADER_FILES})
    file(RELATIVE_PATH REL_PATH "${ENGINE_SRC_DIR}" "${HEADER}")

    set(DEST "${ENGINE_EXPORT_DIR}/${REL_PATH}")

    get_filename_component(DEST_DIR "${DEST}" DIRECTORY)

    add_custom_command(
            TARGET engine PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEST_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${HEADER}" "${DEST}"
    )
endforeach()