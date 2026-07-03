#pragma once

#include "Window.hpp"
#include "Renderer.hpp"
#include "graphics/Image.hpp"
#include "graphics/Texture.hpp"
#include "Camera2D.hpp"

class Application
{
public:
    bool initialize();
    void run();
    void shutdown();

private:
    Window m_window;
    Camera2D m_camera;
    Renderer m_renderer;
    Image m_image;
    Texture m_texture;
};