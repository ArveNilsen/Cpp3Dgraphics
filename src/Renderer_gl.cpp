#include "Renderer.hpp"

#include "Window.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Geometry.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

struct Renderer::Impl
{
    Impl(const Window& window)
        : window_{ window }
    {}

    void init()
    {
        // glad: load all OpenGL function pointers
        if (!gladLoadGL(glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }    

        // z-buffer?
        glEnable(GL_DEPTH_TEST);

        loadShaders();
        setVertexData();
    }

    void resize(int width, int height)
    {
        (void)width;
        (void)height;
    }

    void renderFrame()
    {
        float currentFrame = static_cast<float>(window_.getTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Clear background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // activate shader before setting uniforms
        lightingShader_.use();
        lightingShader_.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader_.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader_.setVec3("lightPos", lightPos);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(
                glm::radians(camera.Zoom), 
                static_cast<float>(800) / static_cast<float>(600), 
                0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader_.setMat4("projection", projection);
        lightingShader_.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader_.setMat4("model", model);

        // render the cube
        cubeVAO_.bind();
        int vao;
        int currId;
        int enabled;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
        currId = static_cast<int>(cubeVAO_.getId());
        assert(vao == currId);
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        assert(enabled == 1);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // also draw the lamp object
        lightCubeShader_.use();
        lightCubeShader_.setMat4("projection", projection);
        lightCubeShader_.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader_.setMat4("model", model);

        lightCubeVAO_.bind();
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
        currId = static_cast<int>(lightCubeVAO_.getId());
        assert(vao == currId);
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        assert(enabled == 1);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void shutdown()
    {
    }

    void loadShaders()
    {
        lightingShader_.loadShaders( "lighting_vert.glsl", "lighting_frag.glsl" );
        lightCubeShader_.loadShaders( "lightCube_vert.glsl", "lightCube_frag.glsl" );
    }

    void setVertexData()
    {
        cubeVAO_.create();
        VBO_.create();

        cubeVAO_.bind();
        VBO_.bind();

        VBO_.setData(std::as_bytes(std::span(Geometry::basic_light_vertices)));

        // position
        cubeVAO_.setAttrib(
                VBO_,
                0,
                3,
                6 * sizeof(float),
                0
        );

        // normal
        cubeVAO_.setAttrib(
                VBO_,
                1,
                3,
                6 * sizeof(float),
                3 * sizeof(float)
        );
#if 0
        // texcoord
        cubeVAO_.setAttrib(
                VBO_,
                2,
                2,
                6 * sizeof(float),
                6 * sizeof(float)
        );
#endif

        lightCubeVAO_.create();
        lightCubeVAO_.bind();

        VBO_.bind();
        lightCubeVAO_.setAttrib(
                VBO_,
                0,
                3,
                6 * sizeof(float),
                0
        );
    }


    const Window& window_;
    float deltaTime { 0 };
    float lastFrame { 0 };
    Shader lightingShader_;
    Shader lightCubeShader_;
    VertexBuffer VBO_;
    VertexArray cubeVAO_, lightCubeVAO_;
    Camera camera;
    glm::vec3 lightPos{1.2f, 1.0f, 2.0f};
};

Renderer::Renderer(const Window& window)
    : pImpl{ std::make_unique<Impl>(window) }
{}

Renderer::~Renderer() = default;

void Renderer::init()
{
    return pImpl->init();
}

void Renderer::resize(int width, int height) noexcept
{   
    (void)width;
    (void)height;
}

void Renderer::renderFrame()
{
    return pImpl->renderFrame();
}

void Renderer::shutdown()
{}
