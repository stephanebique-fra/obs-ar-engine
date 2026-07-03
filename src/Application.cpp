#include "Application.hpp"

bool Application::initialize()
{
    if (!m_window.create("OBS AR Engine", 1280, 720))
        return false;

    if (!m_renderer.initialize(m_window.renderer()))
        return false;

    if (!m_image.load("assets/images/court.jpg"))
        return false;

    if (!m_texture.create(m_window.renderer(), m_image))
        return false;

    return true;
}

void Application::run()
{
    bool running = true;

    while (running)
    {
        running = m_window.processEvents();

        m_renderer.clear();

        m_renderer.draw(m_texture);

        m_renderer.present();
    }
}

void Application::shutdown()
{
    m_window.destroy();
}