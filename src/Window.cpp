#include "Window.hpp"
#include <glad/gl.h>

Window::Window()
{
}

Window::~Window()
{
    destroy();
}

bool Window::create(
    const char* title,
    int width,
    int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_CAMERA))
        return false;

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MAJOR_VERSION,
        4);

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MINOR_VERSION,
        1);

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(
        SDL_GL_DOUBLEBUFFER,
        1);

    m_window = SDL_CreateWindow(
        title,
        width,
        height,
        SDL_WINDOW_OPENGL);

    if (!m_window)
        return false;

    m_glContext =
        SDL_GL_CreateContext(m_window);

    if (!m_glContext)
        return false;

    SDL_GL_MakeCurrent(
        m_window,
        m_glContext);

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
    {
        return false;
    }

    SDL_GL_SetSwapInterval(1);

    m_renderer =
        SDL_CreateRenderer(
            m_window,
            nullptr);

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

    if (m_glContext)
    {
        SDL_GL_DestroyContext(m_glContext);
        m_glContext = nullptr;
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
    SDL_SetRenderDrawColor(
        m_renderer,
        30,
        30,
        30,
        255);

    SDL_RenderClear(m_renderer);
}

void Window::present()
{
    SDL_RenderPresent(m_renderer);
    SDL_GL_SwapWindow(m_window);
}

SDL_Window* Window::window() const
{
    return m_window;
}

SDL_Renderer* Window::renderer() const
{
    return m_renderer;
}

SDL_GLContext Window::glContext() const
{
    return m_glContext;
}