#pragma once

#include "Window.hpp"
#include "Renderer.hpp"
#include "Camera2D.hpp"
#include "Court.hpp"
#include "VideoSource.hpp"

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
    Court m_court;
    VideoSource m_videoSource;

    bool m_isPanning = false;
};
