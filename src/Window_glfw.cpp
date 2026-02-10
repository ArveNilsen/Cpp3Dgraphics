#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <print>
#include <bitset>

#include "WindowConfig.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Geometry.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

struct Window_glfw
{
    Window_glfw(int width, int height, const std::string& title)
        : window { glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr) }
    {}

    ~Window_glfw()
    {
        if (window)
            glfwDestroyWindow(window);
    }

    // Delete copy ctor and assignment
    Window_glfw(const Window_glfw&) = delete;
    Window_glfw& operator=(const Window_glfw&) = delete;

    // Delete move ctor and assignment
    // TODO: Considr move support
    Window_glfw(Window_glfw&&) = delete;
    Window_glfw& operator=(Window_glfw&&) = delete;

    GLFWwindow* window;
};

struct Window::Impl
{
    Impl(WindowConfig& window_config) 
        : wc {window_config},
        window_context{ wc. width_, wc.height_, wc.title_ },
        inputState{},
        input{ window_context.window, inputState }
    {
        if (!window_context.window)
        {
            throw std::runtime_error("Failed to create GLFW window");
        }

        // glad: load all OpenGL function pointers
        if (!gladLoadGL(glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }    

        // z-buffer?
        glEnable(GL_DEPTH_TEST);
    }

    ~Impl() = default;

    bool shouldClose() noexcept {
        if (isClosing)
        {
            glfwSetWindowUserPointer(window_context.window, nullptr);
            return true;
        }
        else
        {
            return false;
        }
    }

    void swapBuffers() const noexcept {
        glfwSwapBuffers(window_context.window);
    }

    void tick()
    {
        // reset input data
        input.beginFrame();
        // time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // clear background
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
                static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 
                0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader_.setMat4("projection", projection);
        lightingShader_.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader_.setMat4("model", model);

        // render the cube
        cubeVAO_.bind();
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
        glDrawArrays(GL_TRIANGLES, 0, 36);

        swapBuffers();
        glfwPollEvents();
        updateCamera();
        // handle events
        if (inputState.closeRequested())
        {
            isClosing = true;
        }
    }

    void setVertexData()
    {
        cubeVAO_.create();
        VBO_.create();

        cubeVAO_.bind();
        VBO_.bind();

        VBO_.setData(std::as_bytes(std::span(Geometry::vertices)));

        // position
        cubeVAO_.setAttrib(
                VBO_,
                0,
                3,
                8 * sizeof(float),
                0
        );

        // normal
        cubeVAO_.setAttrib(
                VBO_,
                1,
                3,
                8 * sizeof(float),
                3 * sizeof(float)
        );
        // texcoord
        cubeVAO_.setAttrib(
                VBO_,
                2,
                2,
                8 * sizeof(float),
                6 * sizeof(float)
        );

        lightCubeVAO_.create();
        lightCubeVAO_.bind();

        VBO_.bind();
        lightCubeVAO_.setAttrib(
                VBO_,
                0,
                3,
                8 * sizeof(float),
                0
        );
    }

    void loadShaders()
    {
        lightingShader_.loadShaders( "lighting_vert.glsl", "lighting_frag.glsl" );
        lightCubeShader_.loadShaders( "lightCube_vert.glsl", "lightCube_frag.glsl" );
    }

    void updateCamera()
    {
        if (inputState.keyDown(InputState::KEY::W))
            camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
        if (inputState.keyDown(InputState::KEY::S))
            camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
        if (inputState.keyDown(InputState::KEY::A))
            camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
        if (inputState.keyDown(InputState::KEY::D))
            camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

        camera.ProcessMouseMovement(inputState.mouseDeltaX, inputState.mouseDeltaY);
    }

    WindowConfig& wc;
    Window_glfw window_context;
    InputState inputState;
    Input input;
    float deltaTime = 0;
    float lastFrame = 0;
    VertexBuffer VBO_;
    VertexArray cubeVAO_, lightCubeVAO_;
    Shader lightingShader_;
    Shader lightCubeShader_;
    Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
    glm::vec3 lightPos{1.2f, 1.0f, 2.0f};
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool isClosing = false;
};

Window::Window(WindowConfig& window_config) 
    : pImpl{std::make_unique<Impl>(window_config)} 
{}

Window::~Window() = default;

bool Window::shouldClose() const noexcept {
    return pImpl->shouldClose();
}

void Window::tick() {
    pImpl->tick();
}

void Window::loadShaders() {
    pImpl->loadShaders();
}

void Window::setVertexData() noexcept {
    pImpl->setVertexData();
}
