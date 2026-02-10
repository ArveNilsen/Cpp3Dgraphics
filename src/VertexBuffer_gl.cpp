#include <glad/gl.h>

#include <span>

#include "VertexBuffer.hpp"

struct VertexBuffer::Impl
{
    Impl()
        : id { 0 }, target { GL_ARRAY_BUFFER }
    {}

    ~Impl() { destroy(); }

    void destroy()
    {
        if (id)
            glDeleteBuffers(1, &id);

        id = 0;
    }

    bool isOk() const noexcept { return id != 0; }

    unsigned int getId() const noexcept { return id; }

    // idempotent
    bool create() noexcept
    {
        if (id != 0)
            return true;

        glGenBuffers(1, &id);
        return (glIsBuffer(id));
    }

    void bind() const noexcept { glBindBuffer(target, id); }

    void unbind() const noexcept { glBindBuffer(target, 0); }

    void setData(std::span<const std::byte> data) 
    { 
        auto size = data.size();
        auto ptr = data.data();
        glBufferData(
                target, 
                static_cast<GLsizeiptr>(size),
                ptr, 
                GL_STATIC_DRAW); 
    }

    void clear() { glBufferData (target, 0, NULL, 0); }

    GLuint id;
    GLenum target;
};


VertexBuffer::VertexBuffer()
    : pImpl { std::make_unique<Impl>() }
{}
VertexBuffer::~VertexBuffer() = default;

bool VertexBuffer::isOk() const noexcept { return pImpl->isOk(); }
unsigned int VertexBuffer::getId() const noexcept { return pImpl->getId(); }
bool VertexBuffer::create() noexcept { return pImpl->create(); }
void VertexBuffer::destroy() noexcept { return pImpl->destroy(); }
void VertexBuffer::bind() noexcept { return pImpl->bind(); }
void VertexBuffer::unbind() noexcept { return pImpl->unbind(); }

void VertexBuffer::setData(std::span<const std::byte> data) noexcept 
{ 
    return pImpl->setData(data); 
}

void VertexBuffer::clear() noexcept { return pImpl->clear(); }
