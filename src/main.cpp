#include "Application.hpp"
#include "WindowConfig.hpp"
#include "Window.hpp"

int main() {
    Application app;

    WindowConfig wc;
    MainWindow window(wc);
    window.loadShaders();
    while (!window.shouldClose())
    {
        window.render();
    }

    return 0;
}

