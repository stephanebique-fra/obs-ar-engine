#include "Window.hpp"

Window::Window()
{
}

Window::~Window()
{
    destroy();
}

bool Window::create(const char* title, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_CAMERA))
        return false;

    m_window = SDL_CreateWindow(title, width, height, 0);

    if (!m_window)
        return false;

    m_renderer = SDL_CreateRenderer(m_window, nullptr);

    if (!m_renderer)
        return false;

    return true;
}

void Window::destroy()
{
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }

    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
}

bool Window::pollEvent(SDL_Event& event)
{
    return SDL_PollEvent(&event);
}

void Window::clear()
{
    SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
    SDL_RenderClear(m_renderer);
}

void Window::present()
{
    SDL_RenderPresent(m_renderer);
}

SDL_Renderer* Window::renderer() const
{
    return m_renderer;
}
