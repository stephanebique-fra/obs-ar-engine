#pragma once

#include "Window.hpp"
#include "Camera2D.hpp"
#include "Court.hpp"
#include "VideoSource.hpp"
#include "Calibration.hpp"
#include <cstddef>
#include "Homography.hpp"
#include "RendererGL.hpp"
#include "Scene.hpp"

class Application
{
public:
    bool initialize();
    void run();
    void shutdown();

private:
    Window m_window;
    Camera2D m_camera;
    Court m_court;
    VideoSource m_videoSource;
    Calibration m_calibration;
    std::size_t m_selectedCalibrationPoint = 0;
    bool m_isPanning = false;
    bool m_isDraggingCalibrationPoint = false;
    bool m_editCalibration = false;
    Homography m_homography;
    RendererGL m_rendererGL;
    Scene m_scene;
};
