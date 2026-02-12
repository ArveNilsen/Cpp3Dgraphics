add_executable(cpp3Dgraphics)

add_library(ImageLoader)

target_sources(
    ImageLoader
    PRIVATE
        ImageLoader.cpp
)

target_include_directories(
    ImageLoader
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(
    ImageLoader
    PRIVATE
        -Wall
        -O3
)

target_sources(
    cpp3Dgraphics
    PRIVATE
        main.cpp
        WindowConfig_glfw.cpp
        Application.cpp
        Window_glfw.cpp
        Renderer_gl.cpp
        VertexArray_gl.cpp
        VertexBuffer_gl.cpp
        Shader.cpp
        Camera.cpp
        ${CMAKE_SOURCE_DIR}/include/src/gl.c
)

find_package(glfw3 3.4 REQUIRED)
find_package(glm REQUIRED)

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
        glm::glm
        ImageLoader
)
