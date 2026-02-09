#pragma once

#include <memory>

class MainWindow final
{
public:
    MainWindow();
    ~MainWindow();

    static void config() noexcept;

    bool shouldClose() const noexcept;

    void render();

    void setVertexData() noexcept;
    void loadShaders();

    static constexpr int SCR_WIDTH  = 800;
    static constexpr int SCR_HEIGHT = 600;

    // instance handlers
    void onKey(int key, int scancode, int action, int mods);
    void onSize(int width, int height);
    void onScroll(double xoffset, double yoffset);
    void onMouse(double xposIn, double yposIn);

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
