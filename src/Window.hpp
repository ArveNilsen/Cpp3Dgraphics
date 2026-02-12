#pragma once

#include <memory>
#include "InputState.hpp"

struct WindowConfig;

class Window final
{
public:
    Window(WindowConfig& window_config);
    ~Window();

    bool shouldClose() const noexcept;

    static constexpr int SCR_WIDTH  = 800;
    static constexpr int SCR_HEIGHT = 600;

    // instance handlers
    void onKey(int key, int scancode, int action, int mods);
    void onSize(int width, int height);
    void onScroll(double xoffset, double yoffset);
    void onMouse(double xposIn, double yposIn);

    double getTime() const noexcept;
    void swapBuffers() const noexcept;
    void pollEvents() const noexcept;

    void requestClose();

    const InputState& input() const noexcept;
    InputState snapshotInput() const noexcept;

    void*

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
