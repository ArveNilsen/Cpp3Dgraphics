#pragma once

#include <memory>

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

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
