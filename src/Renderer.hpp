#pragma once

#include <memory>

class Window;
class Mesh;

// TODO: Consider using a slot-map
struct MeshHandle
{
    std::uint32_t id { 0 };
    explicit operator bool() { return id != 0; }
};

class Renderer
{
public:
    Renderer(const Window& window);
    ~Renderer();

    void init();
    void resize(int width, int height) noexcept;

    MeshHandle upload(const Mesh& mesh) noexcept;

    void renderFrame();
    void shutdown();

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
