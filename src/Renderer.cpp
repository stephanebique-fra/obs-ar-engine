#include "Renderer.hpp"
#include "Camera2D.hpp"
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

void Renderer::draw(
    const Texture& texture,
    const Camera2D& camera,
    float sourceWidth,
    float sourceHeight)
{
    int windowWidth = 0;
    int windowHeight = 0;

    SDL_GetRenderOutputSize(
        m_renderer,
        &windowWidth,
        &windowHeight);

    const float width = sourceWidth * camera.zoom();
    const float height = sourceHeight * camera.zoom();

    SDL_FRect destination =
    {
        (static_cast<float>(windowWidth) - width) * 0.5f + camera.x(),
        (static_cast<float>(windowHeight) - height) * 0.5f + camera.y(),
        width,
        height
    };

    SDL_RenderTexture(
        m_renderer,
        texture.handle(),
        nullptr,
        &destination);
}
