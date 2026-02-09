#pragma once

#include <string>

struct WindowConfig
{
    int width_;
    int height_;
    std::string title_;

    WindowConfig()
        : width_{ 800 },
        height_ { 600 },
        title_  { "Cpp3Dgraphics" }
    {
        hints();
    }

    WindowConfig(int width, int height, std::string_view title)
        : width_{ width },
        height_ { height },
        title_ { std::string(title) }
    {
        hints();
    }

    void hints();
};
