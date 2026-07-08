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

    m_renderer.setLineThickness(0.05f);
    m_videoSource.openDefaultCamera();

    m_camera.reset();
    m_camera.setZoom(DefaultCameraZoom);
    m_calibration.setImagePoint(0, 100.0f, 100.0f);
    m_calibration.setImagePoint(1, 1180.0f, 100.0f);
    m_calibration.setImagePoint(2, 1180.0f, 620.0f);
    m_calibration.setImagePoint(3, 100.0f, 620.0f);

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

            case SDL_EVENT_CAMERA_DEVICE_DENIED:
            case SDL_EVENT_CAMERA_DEVICE_REMOVED:
                m_videoSource.close();
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
                {
                    if (m_editCalibration)
                    {
                        const std::size_t point =
                            m_calibration.findNearestImagePoint(
                                event.button.x,
                                event.button.y,
                                15.0f);

                        if (point != Calibration::PointCount)
                        {
                            m_selectedCalibrationPoint = point;
                            m_isDraggingCalibrationPoint = true;
                        }
                    }
                    else
                    {
                        m_isPanning = true;
                    }
                }
                break;
                
            case SDL_EVENT_MOUSE_BUTTON_UP:
               if (event.button.button == SDL_BUTTON_LEFT)
                {
                    m_isPanning = false;
                    m_isDraggingCalibrationPoint = false;
                }
                break;

            case SDL_EVENT_MOUSE_MOTION:
                if (m_isDraggingCalibrationPoint)
                {
                    m_calibration.moveImagePoint(
                        m_selectedCalibrationPoint,
                        event.motion.xrel,
                        event.motion.yrel);
                }
                else if (m_isPanning && (event.motion.state & SDL_BUTTON_LMASK))
                {
                    m_camera.move(
                        event.motion.xrel / m_camera.zoom(),
                        event.motion.yrel / m_camera.zoom());
                }
                break;

            case SDL_EVENT_KEY_DOWN:
                if (!event.key.repeat)
                {
                    switch (event.key.key)
                {
                    case SDLK_1:
                        m_selectedCalibrationPoint = 0;
                        break;

                    case SDLK_2:
                        m_selectedCalibrationPoint = 1;
                        break;

                    case SDLK_3:
                        m_selectedCalibrationPoint = 2;
                        break;

                    case SDLK_4:
                        m_selectedCalibrationPoint = 3;
                        break;
                    case SDLK_C:
                        m_editCalibration = !m_editCalibration;
                        break;
                    case SDLK_R:
                        m_camera.reset();
                        m_camera.setZoom(DefaultCameraZoom);
                        m_isPanning = false;
                        break;

                    case SDLK_LEFT:
                        m_calibration.moveImagePoint(m_selectedCalibrationPoint, -5.0f, 0.0f);
                        break;

                    case SDLK_RIGHT:
                        m_calibration.moveImagePoint(m_selectedCalibrationPoint, 5.0f, 0.0f);
                        break;

                    case SDLK_UP:
                        m_calibration.moveImagePoint(m_selectedCalibrationPoint, 0.0f, -5.0f);
                        break;

                    case SDLK_DOWN:
                        m_calibration.moveImagePoint(m_selectedCalibrationPoint, 0.0f, 5.0f);
                        break;

                    default:
                        break;
                    }
                }
                break;

            default:
                break;
            }
        }

        m_videoSource.update();

        m_renderer.clear();
        m_renderer.drawBackground(m_videoSource.frame());

        m_renderer.draw(
            m_court,
            m_camera);
        
        m_renderer.drawCalibration(
            m_calibration,
            m_selectedCalibrationPoint,
            10.0f);
        m_renderer.present();
    }
}

void Application::shutdown()
{
    m_videoSource.close();
    m_renderer.destroy();
    m_window.destroy();
}
