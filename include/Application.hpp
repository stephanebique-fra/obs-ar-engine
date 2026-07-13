#pragma once

#include "Window.hpp"
#include "Renderer.hpp"
#include "Camera2D.hpp"
#include "Court.hpp"
#include "VideoSource.hpp"
#include "Calibration.hpp"
#include <cstddef>
#include "Homography.hpp"
#include "Logo.hpp"
#include "RendererGL.hpp"

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
    Calibration m_calibration;
    std::size_t m_selectedCalibrationPoint = 0;
    bool m_isPanning = false;
    bool m_isDraggingCalibrationPoint = false;
    bool m_editCalibration = false;
    Homography m_homography;
    Logo m_logo;
    RendererGL m_rendererGL;
};
