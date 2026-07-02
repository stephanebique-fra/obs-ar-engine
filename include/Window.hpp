#pragma once

#include <SDL3/SDL.h>

class Window
{
public:
    Window();
    ~Window();

    bool create(const char* title, int width, int height);
    void destroy();

    bool processEvents();

    void clear();
    void present();

    SDL_Renderer* renderer() const;

private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
};