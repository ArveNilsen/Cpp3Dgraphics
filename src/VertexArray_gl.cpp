#include <glad/gl.h>

#include "VertexArray.hpp"

class VertexBuffer;

struct VertexArray::Impl
{
    Impl() = default;
    ~Impl() noexcept { destroy(); }
    // Disable copying
    Impl(const Impl&) = delete;
    Impl& operator=(const Impl&) = delete;

    bool isOk() const noexcept { return glIsVertexArray(id); }
    unsigned int getId() const noexcept { return static_cast<unsigned int>(id); }
    void create() noexcept { glGenVertexArrays(1, &id); }
    void destroy() noexcept
    {
        if (id)
            glDeleteVertexArrays(1, &id);
        id = 0;
    }

    void bind() noexcept { glBindVertexArray(id); }
    void unbind() noexcept { glBindVertexArray(0); }

    void setAttrib(
            const VertexBuffer& vertexBuffer, 
            std::uint32_t index, 
            int components, 
            std::size_t strideBytes, 
            std::uintptr_t offsetBytes)
    {
        (void)vertexBuffer;
        glVertexAttribPointer(
                index,
                components,
                GL_FLOAT,
                GL_FALSE,
                static_cast<GLsizei>(strideBytes),
                reinterpret_cast<const void*>(offsetBytes)
        );

        glEnableVertexAttribArray(index);
    }
    
    GLuint id { 0 };
};

VertexArray::VertexArray()
    : pImpl { std::make_unique<Impl>() }
{}

VertexArray::~VertexArray() = default;

bool VertexArray::isOk() const noexcept { return pImpl->isOk(); }
unsigned int VertexArray::getId() const noexcept { return pImpl->getId(); }
void VertexArray::create() noexcept { return pImpl->create(); }
void VertexArray::destroy() noexcept { return pImpl->destroy(); }
void VertexArray::bind() noexcept { return pImpl->bind(); }
void VertexArray::unbind() noexcept { return pImpl->unbind(); }
void VertexArray::setAttrib(
            const VertexBuffer& vertexBuffer, 
            std::uint32_t index, 
            int components, 
            std::size_t strideBytes, 
            std::uintptr_t offsetBytes)
{
    return pImpl->setAttrib(
            vertexBuffer,
            index,
            components,
            strideBytes,
            offsetBytes);
}
