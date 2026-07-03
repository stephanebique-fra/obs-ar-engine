#include "Renderer.hpp"
#include "graphics/Texture.hpp"

bool Renderer::initialize(SDL_Renderer* renderer)
{
    m_renderer = renderer;
    return m_renderer != nullptr;
}

void Renderer::clear()
{
    SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
    SDL_RenderClear(m_renderer);
}

void Renderer::present()
{
    SDL_RenderPresent(m_renderer);
}

void Renderer::draw(const Texture& texture)
{
    SDL_RenderTexture(
        m_renderer,
        texture.handle(),
        nullptr,
        nullptr);
}