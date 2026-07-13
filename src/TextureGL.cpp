#include "TextureGL.hpp"

#include <iostream>

TextureGL::TextureGL()
{
}

TextureGL::~TextureGL()
{
    destroy();
}

bool TextureGL::load(const std::string &filename)
{
    Image image;

    if (!image.load(filename))
    {
        std::cout
            << "Unable to load image : "
            << filename
            << '\n';

        return false;
    }

    if (!create(
            image.width(),
            image.height(),
            GL_RGBA))
    {
        return false;
    }

    update(image.pixels());

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void TextureGL::bind(unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void TextureGL::destroy()
{
    if (m_texture)
    {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
}

int TextureGL::width() const
{
    return m_width;
}

int TextureGL::height() const
{
    return m_height;
}
bool TextureGL::create(
    int width,
    int height,
    GLenum format)
{
    destroy();

    m_width = width;
    m_height = height;
    m_format = format;

    glGenTextures(1, &m_texture);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}
void TextureGL::update(
    const void *pixels)
{
    glBindTexture(
        GL_TEXTURE_2D,
        m_texture);

    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        m_width,
        m_height,
        m_format,
        GL_UNSIGNED_BYTE,
        pixels);

    glBindTexture(
        GL_TEXTURE_2D,
        0);
}
GLenum TextureGL::format() const
{
    return m_format;
}