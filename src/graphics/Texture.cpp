#include "graphics/Texture.hpp"
#include "graphics/Image.hpp"

Texture::Texture()
{
}

Texture::~Texture()
{
    destroy();
}

bool Texture::create(SDL_Renderer* renderer, const Image& image)
{
    destroy();

    SDL_Surface* surface = SDL_CreateSurfaceFrom(
        image.width(),
        image.height(),
        SDL_PIXELFORMAT_RGBA32,
        const_cast<unsigned char*>(image.pixels()),
        image.width() * 4);

    if (!surface)
        return false;

    m_texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);

    return m_texture != nullptr;
}

void Texture::destroy()
{
    if (m_texture)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

SDL_Texture* Texture::handle() const
{
    return m_texture;
}