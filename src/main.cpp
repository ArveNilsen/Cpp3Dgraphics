#include "Application.hpp"
#include "MainWindow.hpp"

int main() {
    Application app;

    MainWindow::config();
    MainWindow window;
    window.setup();
    window.loadShaders();
    while (!window.shouldClose())
    {
        window.time();
        window.render();
    }

    return 0;
}

