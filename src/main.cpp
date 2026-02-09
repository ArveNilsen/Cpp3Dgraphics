#include "Application.hpp"
#include "WindowConfig.hpp"
#include "Window.hpp"

int main() {
    Application app;

    WindowConfig wc;
    Window window(wc);
    window.loadShaders();
    window.setVertexData();
    while (!window.shouldClose())
    {
        window.tick();
    }

    return 0;
}
