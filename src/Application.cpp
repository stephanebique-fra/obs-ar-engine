#include "Application.hpp"

bool Application::initialize()
{
    return m_window.create("OBS AR Engine", 1280, 720);
}

void Application::run()
{
    bool running = true;

    while (running)
    {
        running = m_window.processEvents();

        m_window.clear();

        // Rien à dessiner pour l'instant

        m_window.present();
    }
}

void Application::shutdown()
{
    m_window.destroy();
}