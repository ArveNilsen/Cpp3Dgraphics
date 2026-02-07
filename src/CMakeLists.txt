add_executable(cpp3Dgraphics)

target_sources(
    cpp3Dgraphics
    PRIVATE
        main.cpp
        ${CMAKE_SOURCE_DIR}/include/src/gl.c
)

find_package(glfw3 3.4 REQUIRED)

target_include_directories(
    cpp3Dgraphics
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
    cpp3Dgraphics
    PRIVATE
        project_warnings
        glfw
)
