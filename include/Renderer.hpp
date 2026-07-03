#pragma once

#include <SDL3/SDL.h>

class Texture;

class Renderer
{
public:
    Renderer() = default;

    bool initialize(SDL_Renderer* renderer);

    void clear();
    void present();

    void draw(const Texture& texture);

private:
    SDL_Renderer* m_renderer = nullptr;
};