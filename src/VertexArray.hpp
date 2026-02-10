#pragma once

#include <memory>

class VertexBuffer;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    bool isOk() const noexcept;
    unsigned int getId() const noexcept;
    void create() noexcept;
    void destroy() noexcept;
    void bind() noexcept;
    void unbind() noexcept;
    void setAttrib(
            const VertexBuffer&, 
            std::uint32_t index, 
            int components, 
            std::size_t strideBytes, 
            std::uintptr_t offsetBytes);

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
