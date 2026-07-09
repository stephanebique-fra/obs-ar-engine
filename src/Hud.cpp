#include "Hud.hpp"

#include <iostream>

bool Hud::initialize(SDL_Renderer* renderer)
{
    m_renderer = renderer;

    if (!TTF_Init())
    {
        std::cout << "TTF_Init failed: "
                  << SDL_GetError()
                  << '\n';
        return false;
    }

    m_font = TTF_OpenFont(
        "assets/fonts/Roboto-Regular.ttf",
        18);

    if (!m_font)
    {
        std::cout << "Unable to load font\n";
        return false;
    }

    return true;
}

void Hud::destroy()
{
    if (m_font)
    {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }

    TTF_Quit();
}

void Hud::drawText(
    int,
    int,
    const std::string&)
{
    // À implémenter au prochain sprint
}