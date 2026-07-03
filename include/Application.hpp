#pragma once

#include "Window.hpp"
#include "Renderer.hpp"
#include "graphics/Image.hpp"
#include "graphics/Texture.hpp"

class Application
{
public:
    bool initialize();
    void run();
    void shutdown();

private:
    Window m_window;
    Renderer m_renderer;
    Image m_image;
    Texture m_texture;
};