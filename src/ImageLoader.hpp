#pragma once

#include <string>
#include <expected>

#include "TextureHandle.hpp"

namespace ImageLoader
{

std::expected<TextureHandle, std::string> loadTexture(const std::string& path);

} // namespace ImageLoader
  
