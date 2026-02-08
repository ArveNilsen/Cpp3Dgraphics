#include <stdexcept>
#include "Application.hpp"

#include <GLFW/glfw3.h>

Application::Application()
{
    if (!glfwInit())
    {
        throw std::runtime_error("GLFW could not be initialized.");
    }
}

Application::~Application() noexcept
{
    glfwTerminate();
}
