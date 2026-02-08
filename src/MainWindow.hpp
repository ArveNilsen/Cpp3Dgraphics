#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <string_view>
#include <memory>
#include <array>

#include "Shader.hpp"
#include "Camera.hpp"

struct Window_Destructor final 
{
    void operator()(GLFWwindow* window) const {
        if (window) {
            glfwDestroyWindow(window);
        }
    }
};

class MainWindow final
{
public:
    MainWindow();

    static void config() noexcept {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }

    bool shouldClose() const noexcept;

    void time() noexcept;
    void render() const noexcept;

    void setup() noexcept;
    void loadShaders();

    static constexpr int SCR_WIDTH  = 800;
    static constexpr int SCR_HEIGHT = 600;


private:
    void swapBuffers() const;
    void pollEvents() const;

    // static trampolines for callbacks
    static void key_callback(GLFWwindow* window, int key, int scancode, 
            int action, int mods);
    static void error_callback(int error, const char* description);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

    // instance handlers
    void onKey(int key, int scancode, int action, int mods);
    void onSize(int width, int height);
    void onScroll(double xoffset, double yoffset);
    void onMouse(double xposIn, double yposIn);

    // The native window handle
    using Window_Owner = std::unique_ptr<GLFWwindow, Window_Destructor>;
    Window_Owner window_handle_;
    Shader lightingShader_;
    Shader lightCubeShader_;
    unsigned int VBO_, cubeVAO_, lightCubeVAO_;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;
    Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
    float deltaTime = 0;
    float lastFrame = 0;
    glm::vec3 lightPos{1.2f, 1.0f, 2.0f};
};
