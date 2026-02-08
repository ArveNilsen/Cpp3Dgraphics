#include "MainWindow.hpp"

#include <print>

static constexpr int DEFAULT_WIDTH  = 800;
static constexpr int DEFAULT_HEIGHT = 600;

MainWindow::MainWindow() 
    : window_handle_{ glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Cpp3Dgraphics", 
            NULL, NULL) } {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    if (!window_handle_)
    {
        std::println("()", "Failed to create GLFW window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window_handle_.get());
    glfwSetFramebufferSizeCallback(window_handle_.get(), framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::println("{}", "Failed to initialize GLAD");
        return;
    }    

    glfwSetKeyCallback(window_handle_.get(), key_callback);
    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(window_handle_.get(), framebuffer_size_callback);
}

MainWindow::~MainWindow() {
    glfwTerminate();
}

bool MainWindow::shouldClose() const noexcept {
    return (glfwWindowShouldClose(window_handle_.get()));
}

void MainWindow::swapBuffers() const {
    glfwSwapBuffers(window_handle_.get());
}

void MainWindow::pollEvents() const {
    glfwPollEvents();
}

void MainWindow::key_callback(GLFWwindow* window, int key, int scancode, 
    int action, int mods) {
    (void)scancode;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void MainWindow::error_callback(int error, const char* description) {
    std::println("{}: {}", error, description);
}

void MainWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}
