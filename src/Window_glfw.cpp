#include "WindowConfig.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "InputState.hpp"

#include <GLFW/glfw3.h>

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
        : wc_ {window_config},
        window_context_{ wc_.width_, wc_.height_, wc_.title_ },
        inputState_{},
        input_{ window_context_.window, inputState_ }
    {
        if (!window_context_.window)
        {
            throw std::runtime_error("Failed to create GLFW window");
        }
    }

    ~Impl() = default;

    bool shouldClose() noexcept 
    {
        if (isClosing_)
        {
            glfwSetWindowUserPointer(window_context_.window, nullptr);
            return true;
        }
        else
        {
            return false;
        }
    }

    double getTime() const noexcept
    {
        return glfwGetTime();
    }

    void swapBuffers() const noexcept
    {
        glfwSwapBuffers(window_context_.window);
    }

    void pollEvents() noexcept
    {
        input_.beginFrame();
        glfwPollEvents();
    }

    const InputState& input() const noexcept
    {
        return inputState_;
    }

    InputState snapshotInput() const noexcept
    {
        return inputState_;
    }

    void requestClose()
    {
        isClosing_ = true;
    }

    WindowConfig& wc_;
    Window_glfw window_context_;
    InputState inputState_;
    Input input_;
    bool isClosing_ = false;
};

Window::Window(WindowConfig& window_config) 
    : pImpl{std::make_unique<Impl>(window_config)} 
{}

Window::~Window() = default;

bool Window::shouldClose() const noexcept {
    return pImpl->shouldClose();
}

double Window::getTime() const noexcept {
    return pImpl->getTime();
}

void Window::swapBuffers() const noexcept {
    return pImpl->swapBuffers();
}

void Window::pollEvents() const noexcept {
    return pImpl->pollEvents();
}

const InputState& Window::input() const noexcept
{
    return pImpl->input();
}

InputState Window::snapshotInput() const noexcept
{
    return pImpl->snapshotInput();
}

void Window::requestClose()
{
    return pImpl->requestClose();
}
