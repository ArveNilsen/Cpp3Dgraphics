add_executable(cpp3Dgraphics)

add_library(stbi)

target_sources(
    stbi
    PRIVATE
        stb_image.cpp
)

target_include_directories(
    stbi
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(
    stbi
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
find_package(assimp REQUIRED)

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
        stbi
        assimp
)
