#pragma once

#include "graphics/Image.hpp"
#include "graphics/Texture.hpp"

#include <SDL3/SDL.h>
#include <string>

class Logo
{
public:
    bool load(
        SDL_Renderer* renderer,
        const std::string& filename);

    void setPosition(float x, float y);
    void setSize(float width, float height);

    float x() const;
    float y() const;

    float width() const;
    float height() const;

    const Texture& texture() const;

private:
    Image m_image;
    Texture m_texture;

    float m_x = 0.0f;
    float m_y = 0.0f;

    float m_width = 1.0f;
    float m_height = 1.0f;
};