#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <string_view>
#include <memory>

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
    ~MainWindow();

    bool shouldClose() const noexcept;

    void swapBuffers() const;
    void pollEvents() const;

private:
    // Callbacks for window handling
    static void key_callback(GLFWwindow* window, int key, int scancode, 
            int action, int mods);
    static void error_callback(int error, const char* description);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    // Must be the first data member!
    bool isInitialised { static_cast<bool>(glfwInit()) };
    // The native window handle
    using Window_Owner = std::unique_ptr<GLFWwindow, Window_Destructor>;
    Window_Owner window_handle_;
};
