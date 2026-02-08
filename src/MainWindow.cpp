#include "MainWindow.hpp"

#include <print>

static constexpr int DEFAULT_WIDTH  = 800;
static constexpr int DEFAULT_HEIGHT = 600;

MainWindow::MainWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    win_handle = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, 
            "Cpp3Dgraphics", NULL, NULL);
    if (win_handle == NULL)
    {
        std::println("()", "Failed to create GLFW window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(win_handle);
    glfwSetFramebufferSizeCallback(win_handle, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::println("{}", "Failed to initialize GLAD");
        return;
    }    

    glfwSetKeyCallback(win_handle, key_callback);
    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(win_handle, framebuffer_size_callback);
}

MainWindow::~MainWindow() {
    glfwTerminate();
}

bool MainWindow::shouldClose() const noexcept {
    return (glfwWindowShouldClose(win_handle));
}

void MainWindow::swapBuffers() const {
    glfwSwapBuffers(win_handle);
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
