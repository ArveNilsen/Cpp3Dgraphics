#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

struct InputState
{
    enum class KEY { Q, W, S, A, D };
    /*
     * 0: Quit
     * 1: W
     * 2: S
     * 3: A
     * 4: D
     */
    std::bitset<5> keys{};

    void setQ() { keys.set(0); }
    void setW() { keys.set(1); }
    void setS() { keys.set(2); }
    void setA() { keys.set(3); }
    void setD() { keys.set(4); }
    
    void reset() 
    { 
        keys.reset(); 
        mouseDeltaX = 0;
        mouseDeltaY = 0;
        scrollDelta = 0;
    }

    bool closeRequested()
    {
        return keys.test(0);
    }

    bool keyDown(KEY key)
    {
        switch (key) {
        case KEY::Q:
            return keys.test(0);
        case KEY::W:
            return keys.test(1);
        case KEY::S:
            return keys.test(2);
        case KEY::A:
            return keys.test(3);
        case KEY::D:
            return keys.test(4);
        default:
            return false;
        }
    }

    // TODO: bool pressed(key) -> toggles
    float mouseDeltaX{ 0 };
    float mouseDeltaY{ 0 };
    float scrollDelta{ 0 };
    // TODO: Clamp deltas to prevent big jumps
};

struct Input 
{
    Input(GLFWwindow* window, InputState& inputState)
        : window_{ window },
        inputState_{ inputState }
    {
        glfwMakeContextCurrent(window_);
        
        glfwSetWindowUserPointer(window_, this);

        // register the callback trampolines
        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
        glfwSetKeyCallback(window_, &key_callback);
        glfwSetErrorCallback(error_callback);
        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
        glfwSetCursorPosCallback(window_, mouse_callback);
        glfwSetScrollCallback(window_, scroll_callback);

        // enable mouse movement
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void beginFrame()
    {
        inputState_.reset();
    }

    void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        (void)window;
        (void)scancode;
        (void)mods;

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            inputState_.setQ();
        if (key == GLFW_KEY_W && action == GLFW_PRESS)
            inputState_.setW();
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
            inputState_.setS();
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
            inputState_.setA();
        if (key == GLFW_KEY_D && action == GLFW_PRESS)
            inputState_.setD();
    }


    void onSize(GLFWwindow* window, int width, int height)
    {
        (void)window;
        glViewport(0, 0, width, height);
    }

    void onScroll(GLFWwindow* window, double xoffset, double yoffset)
    {
        (void)window;
        (void)xoffset;
        (void)yoffset;
    }

    void onMouse(GLFWwindow* window, double xposIn, double yposIn)
    {
        (void)window;
        (void)xposIn;
        (void)yposIn;

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            mouseLastX = xpos;
            mouseLastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - mouseLastX;
        float yoffset = mouseLastY - ypos; // reversed since y-coordinates go from bottom to top

        mouseLastX = xpos;
        mouseLastY = ypos;

        // send xoffset and yoffset to the InputState
        inputState_.mouseDeltaX += xoffset;
        inputState_.mouseDeltaY += yoffset;
    }
    
    // static trampolines for callbacks
    static void key_callback(GLFWwindow* window, int key, int scancode, 
            int action, int mods)
    {
        auto* self = 
            static_cast<Input*>(glfwGetWindowUserPointer(window));

        if (!self) {
            return;
        }

        self->onKey(window, key, scancode, action, mods);
    }

    static void error_callback(int error, const char* description)
    {
        std::println("{}: {}", error, description);
    }

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        auto* self = 
            static_cast<Input*>(glfwGetWindowUserPointer(window));

        if (!self) {
            return;
        }

        self->onSize(window, width, height);
    }

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        auto* self = 
            static_cast<Input*>(glfwGetWindowUserPointer(window));

        if (!self) {
            return;
        }

        self->onScroll(window, xoffset, yoffset);
    }

    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
    {
        auto* self = 
            static_cast<Input*>(glfwGetWindowUserPointer(window));

        if (!self) {
            return;
        }

        self->onMouse(window, xposIn, yposIn);
    }

    GLFWwindow* window_;
    InputState& inputState_;
    float mouseLastX{ 0 };
    float mouseLastY{ 0 };
    bool firstMouse = true;
};


