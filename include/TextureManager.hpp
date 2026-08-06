#pragma once

#include <vector>
#include "TextureGL.hpp"

class TextureManager
{
public:
    bool initialize();

    const TextureGL& texture(std::size_t id) const;

private:
    std::vector<TextureGL> m_textures;
};