#include "Application.hpp"
#include <vector>
#include <opencv2/core.hpp>
#include <iostream>
#include "FibaCourt.hpp"

namespace
{
    constexpr float DefaultCameraZoom = 1.0f;
    constexpr float WheelZoomFactor = 1.1f;
}

bool Application::initialize()
{
    if (!m_window.create("OBS AR Engine", 1280, 720))
        return false;

    if (!m_rendererGL.initialize())
        return false;

    m_videoSource.openDefaultCamera();

    m_camera.reset();
    m_camera.setZoom(DefaultCameraZoom);

    // Charger une calibration existante
    if (!m_calibration.load("calibration.txt"))
    {
        // Sinon créer la calibration par défaut
        m_calibration.addPoint({FibaMarker::CourtTopLeft,
                                100.0f,
                                100.0f,
                                0.0f,
                                0.0f,
                                true});

        m_calibration.addPoint({FibaMarker::CourtTopRight,
                                1180.0f,
                                100.0f,
                                28.0f,
                                0.0f,
                                true});

        m_calibration.addPoint({FibaMarker::CourtBottomRight,
                                1180.0f,
                                620.0f,
                                28.0f,
                                15.0f,
                                true});

        m_calibration.addPoint({FibaMarker::CourtBottomLeft,
                                100.0f,
                                620.0f,
                                0.0f,
                                15.0f,
                                true});

        m_calibration.save("calibration.txt");
    }
    m_scene.advertisements().push_back({
        10.0f,
        6.0f,
        4.0f,
        2.0f,
        true
    });

    m_scene.advertisements().push_back({
        3.0f,
        2.0f,
        2.5f,
        1.2f,
        true
    });
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
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (m_editCalibration)
                    {
                        const std::size_t point =
                            m_calibration.findNearestImagePoint(
                                event.button.x,
                                event.button.y,
                                20.0f);

                        if (point < m_calibration.pointCount())
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

                if (event.button.button == SDL_BUTTON_RIGHT &&
                    m_editCalibration)
                {
                    const auto &p =
                        m_calibration.point(m_selectedCalibrationPoint);

                    m_calibration.addPoint(
                        p.imageX + 25.0f,
                        p.imageY + 25.0f);

                    m_selectedCalibrationPoint =
                        m_calibration.pointCount() - 1;
                }

                break;
            }

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
                    m_calibration.movePoint(
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
                    case SDLK_TAB:
                    {
                        if (event.key.mod & SDL_KMOD_SHIFT)
                        {
                            if (m_selectedCalibrationPoint == 0)
                                m_selectedCalibrationPoint = m_calibration.pointCount() - 1;
                            else
                                --m_selectedCalibrationPoint;
                        }
                        else
                        {
                            ++m_selectedCalibrationPoint;

                            if (m_selectedCalibrationPoint >= m_calibration.pointCount())
                                m_selectedCalibrationPoint = 0;
                        }
                        break;
                    }

                    case SDLK_C:
                        m_editCalibration = !m_editCalibration;
                        break;
                    case SDLK_R:
                        m_camera.reset();
                        m_camera.setZoom(DefaultCameraZoom);
                        m_isPanning = false;
                        break;

                    case SDLK_N:
                    {
                        m_calibration.addPoint(640.0f, 360.0f);
                        m_selectedCalibrationPoint =
                            m_calibration.pointCount() - 1;
                        break;
                    }
                    case SDLK_DELETE:
                    case SDLK_BACKSPACE:
                    {
                        if (m_calibration.pointCount() > 4)
                        {
                            m_calibration.removePoint(m_selectedCalibrationPoint);

                            if (m_selectedCalibrationPoint >= m_calibration.pointCount())
                                m_selectedCalibrationPoint =
                                    m_calibration.pointCount() - 1;
                        }

                        break;
                    }
                    case SDLK_E:
                        m_calibration.togglePoint(
                            m_selectedCalibrationPoint);
                        break;
                    case SDLK_F:
                        m_calibration.centerPoint(
                            m_selectedCalibrationPoint,
                            640.0f,
                            360.0f);
                        break;
                    case SDLK_LEFT:
                        m_calibration.movePoint(m_selectedCalibrationPoint, -5.0f, 0.0f);
                        break;

                    case SDLK_RIGHT:
                        m_calibration.movePoint(m_selectedCalibrationPoint, 5.0f, 0.0f);
                        break;

                    case SDLK_UP:
                        m_calibration.movePoint(m_selectedCalibrationPoint, 0.0f, -5.0f);
                        break;

                    case SDLK_DOWN:
                        m_calibration.movePoint(m_selectedCalibrationPoint, 0.0f, 5.0f);
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

        std::vector<cv::Point2f> imagePoints;
        std::vector<cv::Point2f> courtPoints;

        for (std::size_t i = 0; i < m_calibration.pointCount(); ++i)
        {
            const auto &point = m_calibration.point(i);

            if (!point.enabled)
                continue;

            imagePoints.emplace_back(point.imageX, point.imageY);
            const Court::Point courtPosition =
                FibaCourt::markerPosition(point.marker);
            std::cout
                << "Marker = " << static_cast<int>(point.marker)
                << " -> "
                << courtPosition.x << ", "
                << courtPosition.y
                << '\n';

            courtPoints.emplace_back(
                courtPosition.x,
                courtPosition.y);
        }
        std::cout << "Image points : " << imagePoints.size() << '\n';
        std::cout << "Court points : " << courtPoints.size() << '\n';

        const bool ok =
            m_homography.compute(imagePoints, courtPoints);

        std::cout << "compute() = "
                  << (ok ? "OK" : "FAILED")
                  << '\n';

        if (!m_window.makeGLCurrent())
            continue;

        std::cout << "context before GL: expected="
                  << m_window.glContext()
                  << " current=" << SDL_GL_GetCurrentContext()
                  << " matches="
                  << (m_window.glContext() == SDL_GL_GetCurrentContext())
                  << '\n';

        int pixelWidth = 0;
        int pixelHeight = 0;

        if (!m_window.pixelSize(pixelWidth, pixelHeight))
            continue;

        m_rendererGL.beginFrame(pixelWidth, pixelHeight);

        m_rendererGL.drawBackground(m_videoSource.frame());

        std::cout
            << "Homography valid = "
            << m_homography.isValid()
            << std::endl;

        const auto &frame = m_videoSource.frame();

        std::cout
            << frame.width
            << " x "
            << frame.height
            << " valid="
            << frame.isValid()
            << std::endl;

        m_rendererGL.drawProjectedCourt(
            m_court,
            m_homography);

        m_rendererGL.drawProjectedRectangle(
            10.0f,
            6.0f,
            4.0f,
            4.0f,
            m_homography);

        for (const Advertisement &ad : m_scene.advertisements())
        {
            m_rendererGL.drawProjectedTexture(
                ad.x,
                ad.y,
                ad.width,
                ad.height,
                m_homography);
        }

        std::cout << "context before present: expected="
                  << m_window.glContext()
                  << " current=" << SDL_GL_GetCurrentContext()
                  << " matches="
                  << (m_window.glContext() == SDL_GL_GetCurrentContext())
                  << '\n';

        m_rendererGL.endFrame();

        m_window.present();
    }
}

void Application::shutdown()
{
    m_calibration.save("calibration.txt");
    m_videoSource.close();
    m_window.destroy();
}
