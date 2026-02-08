#include "MainWindow.hpp"

#include <print>

static constexpr std::array<float, 216> vertices_  = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


MainWindow::MainWindow() 
    : window_handle_{ glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cpp3Dgraphics", 
            NULL, NULL) } {

    if (!window_handle_)
    {
        std::println("()", "Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    glfwSetWindowUserPointer(window_handle_.get(), this);

    glfwMakeContextCurrent(window_handle_.get());
    glfwSetFramebufferSizeCallback(window_handle_.get(), framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::println("{}", "Failed to initialize GLAD");
        return;
    }    

    glfwSetKeyCallback(
            window_handle_.get(), 
            &MainWindow::key_callback
    );

    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(window_handle_.get(), framebuffer_size_callback);
    glfwSetCursorPosCallback(window_handle_.get(), mouse_callback);
    glfwSetScrollCallback(window_handle_.get(), scroll_callback);

    glfwSetInputMode(window_handle_.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
}

bool MainWindow::shouldClose() const noexcept {
    return (glfwWindowShouldClose(window_handle_.get()));
}

void MainWindow::time() noexcept {
    // time logic
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

}

void MainWindow::render() const noexcept {
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

void MainWindow::loadShaders() {
    lightingShader_.loadShaders( "lighting_vert.glsl", "lighting_frag.glsl" );
    lightCubeShader_.loadShaders( "lightCube_vert.glsl", "lightCube_frag.glsl" );
}

void MainWindow::setup() noexcept {
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

void MainWindow::swapBuffers() const {
    glfwSwapBuffers(window_handle_.get());
}

void MainWindow::pollEvents() const {
    glfwPollEvents();
}

void MainWindow::key_callback(
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

void MainWindow::onKey(
        int key, 
        int scancode, 
        int action, 
        int mods) 
{
    (void)scancode;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window_handle_.get(), true);

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

void MainWindow::error_callback(
        int error, 
        const char* description) 
{
    std::println("{}: {}", error, description);
}

void MainWindow::framebuffer_size_callback(
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

void MainWindow::onSize(
        int width, 
        int height)
{
    glViewport(0, 0, width, height);
}

void MainWindow::mouse_callback(GLFWwindow* window, 
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

void MainWindow::onMouse(
        double xposIn, 
        double yposIn)
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

void MainWindow::scroll_callback(
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

void MainWindow::onScroll(
        double xoffset, 
        double yoffset) 
{
    (void)xoffset;
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
