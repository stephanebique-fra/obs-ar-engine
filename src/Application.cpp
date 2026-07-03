#include "Application.hpp"
#include <iostream>

bool Application::initialize()
{
    std::cout << "1\n";

    if (!m_window.create("OBS AR Engine", 1280, 720))
    {
        std::cout << "Window failed\n";
        return false;
    }

    std::cout << "2\n";

    if (!m_renderer.initialize(m_window.renderer()))
    {
        std::cout << "Renderer failed\n";
        return false;
    }

    std::cout << "3\n";

    if (!m_image.load("assets/images/court.jpg"))
    {
        std::cout << "Image failed\n";
        return false;
    }

    std::cout << "4\n";

    if (!m_texture.create(m_window.renderer(), m_image))
    {
        std::cout << "Texture failed\n";
        return false;
    }

    std::cout << "5\n";

    m_camera.reset();
    m_camera.setZoom(2.0f);

    std::cout << "OK\n";

    return true;
}

void Application::run()
{
    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (m_window.pollEvent(event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        m_renderer.clear();

        m_renderer.draw(
            m_texture,
            m_camera.x(),
            m_camera.y(),
            static_cast<float>(m_image.width()) * m_camera.zoom(),
            static_cast<float>(m_image.height()) * m_camera.zoom());

        m_renderer.present();
    }
}

void Application::shutdown()
{
    m_window.destroy();
}