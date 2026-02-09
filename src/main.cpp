#include "Application.hpp"
#include "Window.hpp"

int main() {
    Application app;

    MainWindow::config();
    MainWindow window;
    window.loadShaders();
    while (!window.shouldClose())
    {
        window.render();
    }

    return 0;
}

