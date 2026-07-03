#pragma once

#include <SDL3/SDL.h>

class Image;

class Texture
{
public:
    Texture();
    ~Texture();

    bool create(SDL_Renderer* renderer, const Image& image);

    void destroy();

    SDL_Texture* handle() const;

private:
    SDL_Texture* m_texture = nullptr;
};