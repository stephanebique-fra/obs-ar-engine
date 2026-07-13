#pragma once

#include "graphics/Image.hpp"

#include <glad/gl.h>
#include <string>

class TextureGL
{
public:
    TextureGL();
    ~TextureGL();

    bool load(const std::string& filename);

    void bind(unsigned int unit = 0) const;

    void destroy();

    int width() const;
    int height() const;

    bool create(
        int width,
        int height,
        GLenum format);

    void update(
        const void* pixels);
    GLenum format() const;   

private:
    GLuint m_texture = 0;

    int m_width = 0;
    int m_height = 0;
    GLenum m_format = GL_RGBA;
};