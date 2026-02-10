#pragma once

#include <memory>
#include <span>

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    bool isOk() const noexcept;
    unsigned int getId() const noexcept;
    bool create() noexcept;
    void destroy() noexcept;
    void bind() noexcept;
    void unbind() noexcept;

    void setData(std::span<const std::byte> data) noexcept;
    void clear() noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
