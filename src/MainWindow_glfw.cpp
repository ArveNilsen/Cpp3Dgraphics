#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <print>

#include "MainWindow.hpp"
#include "Geometry.hpp"
#include "Shader.hpp"
#include "Camera.hpp"


namespace {
// static trampolines for callbacks
static void key_callback(GLFWwindow* window, int key, int scancode, 
        int action, int mods);
static void error_callback(int error, const char* description);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

} // anon namespace


struct Window_glfw
{
    Window_glfw() : window{ glfwCreateWindow(
            MainWindow::SCR_WIDTH, 
            MainWindow::SCR_HEIGHT, "Cpp3Dgraphics",
            nullptr, nullptr) } {}

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

struct MainWindow::Impl
{
    Impl() : window_context{}
    {
        if (!window_context.window)
        {
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(window_context.window);
        
        glfwSetWindowUserPointer(window_context.window, this);

        // register the callback trampolines
        glfwSetFramebufferSizeCallback(window_context.window, framebuffer_size_callback);
        glfwSetKeyCallback(window_context.window, &key_callback);
        glfwSetErrorCallback(error_callback);
        glfwSetFramebufferSizeCallback(window_context.window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window_context.window, mouse_callback);
        glfwSetScrollCallback(window_context.window, scroll_callback);

        // enable mouse movement
        glfwSetInputMode(window_context.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
        return (glfwWindowShouldClose(window_context.window));
    }

    void swapBuffers() const noexcept {
        glfwSwapBuffers(window_context.window);
    }

    void pollEvents() const noexcept {
        glfwPollEvents();
    }

    void onKey(int key, int scancode, int action, int mods)
    {
        (void)scancode;
        (void)mods;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window_context.window, true);

#if 0
        if (key == GLFW_KEY_W && action == GLFW_PRESS)
            //camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
            //camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
            //camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
        if (key == GLFW_KEY_D && action == GLFW_PRESS)
            //camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
#endif
    }

    void onSize(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void render()
    {
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
        glBindVertexArray(cubeVAO_);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // also draw the lamp object
        lightCubeShader_.use();
        lightCubeShader_.setMat4("projection", projection);
        lightCubeShader_.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader_.setMat4("model", model);

        glBindVertexArray(lightCubeVAO_);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        swapBuffers();
        pollEvents();
    }

    void setVertexData()
    {
        glGenVertexArrays(1, &cubeVAO_);
        glGenBuffers(1, &VBO_);


        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferData(GL_ARRAY_BUFFER, 
                static_cast<GLsizeiptr>(vertices_.size() * sizeof(float)),
                vertices_.data(), GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO_);

        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
        glEnableVertexAttribArray(0);
        // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
                reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenVertexArrays(1, &lightCubeVAO_);
        glBindVertexArray(lightCubeVAO_);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
        glEnableVertexAttribArray(0);
    }

    void loadShaders()
    {
        lightingShader_.loadShaders( "lighting_vert.glsl", "lighting_frag.glsl" );
        lightCubeShader_.loadShaders( "lightCube_vert.glsl", "lightCube_frag.glsl" );
    }

    void onMouse(double xposIn, double yposIn)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }

    void onScroll(double xoffset, double yoffset)
    {  
        (void)xoffset;
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }


    Window_glfw window_context;
    float deltaTime = 0;
    float lastFrame = 0;
    unsigned int VBO_, cubeVAO_, lightCubeVAO_;
    Shader lightingShader_;
    Shader lightCubeShader_;
    Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
    glm::vec3 lightPos{1.2f, 1.0f, 2.0f};
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

};

MainWindow::MainWindow() 
    : pImpl{std::make_unique<Impl>()} 
{
}

MainWindow::~MainWindow() = default;

bool MainWindow::shouldClose() const noexcept {
    return pImpl->shouldClose();
}

// TODO: Move this into a config object
void MainWindow::config() noexcept {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void MainWindow::render() {
    pImpl->render();
}

void MainWindow::loadShaders() {
    pImpl->loadShaders();
}

void MainWindow::setVertexData() noexcept {
    pImpl->setVertexData();
}

void MainWindow::onKey(
        int key, 
        int scancode, 
        int action, 
        int mods) 
{
    pImpl->onKey(key, scancode, action, mods);
}

void MainWindow::onSize(
        int width, 
        int height)
{
    pImpl->onSize(width, height);
}

void MainWindow::onMouse(
        double xposIn, 
        double yposIn)
{
    pImpl->onMouse(xposIn, yposIn);
}

void MainWindow::onScroll(
        double xoffset, 
        double yoffset) 
{
    pImpl->onScroll(xoffset, yoffset);
}

namespace {
// Implementations of the trampoline callbacks
void key_callback(
        GLFWwindow* window, 
        int key, 
        int scancode, 
        int action, 
        int mods) 
{
    auto* self = 
        static_cast<MainWindow*>(glfwGetWindowUserPointer(window));

    if (!self) {
        return;
    }

    self->onKey(key, scancode, action, mods);
}

void error_callback(
        int error, 
        const char* description) 
{
    std::println("{}: {}", error, description);
}

void framebuffer_size_callback(
        GLFWwindow* window, 
        int width, 
        int height) 
{
    auto* self = 
        static_cast<MainWindow*>(glfwGetWindowUserPointer(window));

    if (!self) {
        return;
    }

    self->onSize(width, height);
}

void mouse_callback(GLFWwindow* window, 
        double xposIn, 
        double yposIn) 
{

    auto* self = 
        static_cast<MainWindow*>(glfwGetWindowUserPointer(window));

    if (!self) {
        return;
    }

    self->onMouse(xposIn, yposIn);
}

void scroll_callback(
        GLFWwindow* window, 
        double xoffset, 
        double yoffset) 
{
    auto* self = 
        static_cast<MainWindow*>(glfwGetWindowUserPointer(window));

    if (!self) {
        return;
    }

    self->onScroll(xoffset, yoffset);
}
} // anon namespace
