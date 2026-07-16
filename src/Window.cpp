#include "Window.hpp"
#include <glad/gl.h>
#include <iostream>

Window::Window()
{
}

Window::~Window()
{
    destroy();
}

bool Window::create(
    const char *title,
    int width,
    int height)
{
    std::cout << "========== WINDOW CREATE ==========" << std::endl;
    
    m_width = width;
    m_height = height;

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

    std::cout << "Vendor   : "
              << glGetString(GL_VENDOR)
              << std::endl;

    std::cout << "Renderer : "
              << glGetString(GL_RENDERER)
              << std::endl;

    std::cout << "Version  : "
              << glGetString(GL_VERSION)
              << std::endl;

    GLint vao = 0;

    glGetIntegerv(
        GL_VERTEX_ARRAY_BINDING,
        &vao);

    std::cout
        << "Current VAO = "
        << vao
        << std::endl;

    SDL_GL_SetSwapInterval(1);

    return true;
}

void Window::destroy()
{
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

bool Window::pollEvent(SDL_Event &event)
{
    return SDL_PollEvent(&event);
}

void Window::clear()
{
    glViewport(
        0,
        0,
        m_width,
        m_height);

    glClearColor(
        0.12f,
        0.12f,
        0.12f,
        1.0f);

    glClear(
        GL_COLOR_BUFFER_BIT);
}

void Window::present()
{
    std::cout << "context at present entry: expected="
              << m_glContext
              << " current=" << SDL_GL_GetCurrentContext()
              << " matches="
              << (m_glContext == SDL_GL_GetCurrentContext())
              << '\n';

    if (!makeGLCurrent())
        return;

    GLint viewport[4] = {};
    GLint drawFramebuffer = 0;
    GLint readFramebuffer = 0;
    GLubyte centerPixel[4] = {};

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebuffer);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer);
    glReadPixels(
        viewport[2] / 2,
        viewport[3] / 2,
        1,
        1,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        centerPixel);

    std::cout << "context at swap: expected="
              << m_glContext
              << " current=" << SDL_GL_GetCurrentContext()
              << " matches="
              << (m_glContext == SDL_GL_GetCurrentContext())
              << " framebuffer=" << drawFramebuffer << '/' << readFramebuffer
              << " viewport=" << viewport[0] << ',' << viewport[1]
              << ' ' << viewport[2] << 'x' << viewport[3]
              << " center-pixel="
              << static_cast<int>(centerPixel[0]) << ','
              << static_cast<int>(centerPixel[1]) << ','
              << static_cast<int>(centerPixel[2]) << ','
              << static_cast<int>(centerPixel[3])
              << " read-error=" << glGetError()
              << '\n';

    const bool swapped = SDL_GL_SwapWindow(m_window);

    std::cout << "SDL_GL_SwapWindow: success=" << swapped
              << " error='" << SDL_GetError() << "'\n";
}

bool Window::makeGLCurrent() const
{
    return SDL_GL_MakeCurrent(
        m_window,
        m_glContext);
}

bool Window::pixelSize(int& width, int& height) const
{
    return SDL_GetWindowSizeInPixels(
        m_window,
        &width,
        &height);
}

SDL_Window *Window::window() const
{
    return m_window;
}

SDL_GLContext Window::glContext() const
{
    return m_glContext;
}
