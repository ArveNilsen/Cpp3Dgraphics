#include "ImageLoader.hpp"

#include <glad/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ImageLoader {

std::expected<TextureHandle, std::string> loadTexture(const std::string& path)
{
    TextureHandle textureHandle;
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format { 0 };
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
        {
            return std::unexpected("Invalid number of components in image data");
        }

        glBindTexture(GL_TEXTURE_2D, textureHandle.id);
        glTexImage2D(GL_TEXTURE_2D, 0, 
                static_cast<GLint>(format),
                width, height, 0, format, 
                GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        return std::unexpected("Failed to load texture");
    }

    return textureHandle;
}
} // namespace ImageLoader
