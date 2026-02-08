#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <string_view>

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

    // The native window handle
    GLFWwindow* win_handle = nullptr;
};
