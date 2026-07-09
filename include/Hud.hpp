#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class Hud
{
public:
    bool initialize(SDL_Renderer* renderer);
    void destroy();

    void drawText(
        int x,
        int y,
        const std::string& text);

private:
    SDL_Renderer* m_renderer = nullptr;
    TTF_Font* m_font = nullptr;
};