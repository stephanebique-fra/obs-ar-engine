#pragma once

#include <SDL3/SDL.h>

class Window
{
public:
    Window();
    ~Window();

    bool create(const char* title, int width, int height);
    void destroy();

    bool pollEvent(SDL_Event& event);

    void clear();
    void present();
    bool makeGLCurrent() const;
    bool pixelSize(int& width, int& height) const;

    SDL_Window* window() const;
    SDL_GLContext glContext() const;

private:
    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    int m_width = 0;
    int m_height = 0;
};
