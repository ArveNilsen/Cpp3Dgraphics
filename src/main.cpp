#include "MainWindow.hpp"

int main() {
    MainWindow window;
    while (!window.shouldClose())
    {
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}

