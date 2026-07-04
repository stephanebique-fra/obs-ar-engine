#pragma once

#include <SDL3/SDL.h>

class Texture;
class Camera2D;

class Renderer
{
public:
    Renderer() = default;

    bool initialize(SDL_Renderer* renderer);

    void clear();
    void present();

    void draw(
        const Texture& texture,
        const Camera2D& camera,
        float sourceWidth,
        float sourceHeight);

private:
    SDL_Renderer* m_renderer = nullptr;
};
