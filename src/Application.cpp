#include "Application.hpp"


namespace
{
constexpr float DefaultCameraZoom = 1.0f;
constexpr float WheelZoomFactor = 1.1f;
}

bool Application::initialize()
{

    if (!m_window.create("OBS AR Engine", 1280, 720))
    {
        return false;
    }


    if (!m_renderer.initialize(m_window.renderer()))
    {
        return false;
    }

    m_camera.reset();
    m_camera.setZoom(DefaultCameraZoom);


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
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;

            case SDL_EVENT_MOUSE_WHEEL:
            {
                float wheelY = event.wheel.y;

                if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
                    wheelY *= -1.0f;

                if (wheelY > 0.0f)
                    m_camera.zoom(WheelZoomFactor);
                else if (wheelY < 0.0f)
                    m_camera.zoom(1.0f / WheelZoomFactor);

                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    m_isPanning = true;
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == SDL_BUTTON_LEFT)
                    m_isPanning = false;
                break;

            case SDL_EVENT_MOUSE_MOTION:
                if (m_isPanning && (event.motion.state & SDL_BUTTON_LMASK))
                    m_camera.move(
                        event.motion.xrel / m_camera.zoom(),
                        event.motion.yrel / m_camera.zoom());
                break;

            case SDL_EVENT_KEY_DOWN:
                if (!event.key.repeat && event.key.key == SDLK_R)
                {
                    m_camera.reset();
                    m_camera.setZoom(DefaultCameraZoom);
                    m_isPanning = false;
                }
                break;

            default:
                break;
            }
        }

        m_renderer.clear();

        m_renderer.draw(
            m_court,
            m_camera);

        m_renderer.present();
    }
}

void Application::shutdown()
{
    m_window.destroy();
}
