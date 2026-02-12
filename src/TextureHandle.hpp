#pragma once

struct TextureHandle
{
    std::uint32_t id { 0 };

    explicit operator bool()
    {
        return id != 0;
    }
};

