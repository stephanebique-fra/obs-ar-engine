#include "Logo.hpp"

bool Logo::load(
    SDL_Renderer* renderer,
    const std::string& filename)
{
    if (!m_image.load(filename))
        return false;

    return m_texture.create(
        renderer,
        m_image);
}

void Logo::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
}

void Logo::setSize(float width, float height)
{
    m_width = width;
    m_height = height;
}

float Logo::x() const
{
    return m_x;
}

float Logo::y() const
{
    return m_y;
}

float Logo::width() const
{
    return m_width;
}

float Logo::height() const
{
    return m_height;
}

const Texture& Logo::texture() const
{
    return m_texture;
}