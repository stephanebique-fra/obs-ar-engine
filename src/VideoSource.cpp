#include "VideoSource.hpp"

#include <SDL3/SDL.h>
#include <cstring>
#include <iostream>
#include <string>

namespace
{
const char* permissionStateName(SDL_CameraPermissionState state)
{
    switch (state)
    {
    case SDL_CAMERA_PERMISSION_STATE_DENIED:
        return "denied";
    case SDL_CAMERA_PERMISSION_STATE_PENDING:
        return "pending";
    case SDL_CAMERA_PERMISSION_STATE_APPROVED:
        return "approved";
    default:
        return "unknown";
    }
}
}

VideoSource::~VideoSource()
{
    close();
}

bool VideoSource::openDefaultCamera()
{
    close();

    int cameraCount = 0;
    SDL_CameraID* cameras = SDL_GetCameras(&cameraCount);

    std::cout << "Detected cameras: " << cameraCount << '\n';

    if (!cameras || cameraCount == 0)
    {
        SDL_free(cameras);
        return false;
    }

    SDL_CameraID selectedCamera = cameras[0];
    const char* selectedCameraName = SDL_GetCameraName(selectedCamera);

    for (int i = 0; i < cameraCount; ++i)
    {
        const char* cameraName = SDL_GetCameraName(cameras[i]);
        std::cout << "Camera " << i << ": " << (cameraName ? cameraName : "unknown") << '\n';

        if (cameraName && std::string(cameraName).find("MacBook") != std::string::npos)
        {
            selectedCamera = cameras[i];
            selectedCameraName = cameraName;
        }
    }

    std::cout << "Opening camera: " << (selectedCameraName ? selectedCameraName : "unknown") << '\n';

    SDL_CameraSpec spec = {};
    spec.format = SDL_PIXELFORMAT_RGBA32;
    spec.width = 1280;
    spec.height = 720;
    spec.framerate_numerator = 30;
    spec.framerate_denominator = 1;

    m_camera = SDL_OpenCamera(selectedCamera, &spec);
    SDL_free(cameras);

    if (!m_camera)
    {
        std::cout << "SDL_OpenCamera failed: " << SDL_GetError() << '\n';
        return false;
    }

    const SDL_CameraPermissionState permissionState = SDL_GetCameraPermissionState(m_camera);
    std::cout << "Camera permission state: " << permissionStateName(permissionState) << '\n';

    return true;
}

void VideoSource::close()
{
    if (m_camera)
    {
        SDL_CloseCamera(m_camera);
        m_camera = nullptr;
    }

    ++m_frame.version;
    m_frame.width = 0;
    m_frame.height = 0;
    m_frame.pitch = 0;
    m_frame.pixels.clear();
}

void VideoSource::update()
{
    static bool loggedAcquireFailure = false;
    static bool loggedFirstFrame = false;

    if (!m_camera)
        return;

    if (SDL_GetCameraPermissionState(m_camera) != SDL_CAMERA_PERMISSION_STATE_APPROVED)
        return;

    Uint64 timestamp = 0;
    SDL_Surface* frame = SDL_AcquireCameraFrame(m_camera, &timestamp);

    if (!frame)
    {
        if (!loggedAcquireFailure)
        {
            std::cout << "SDL_AcquireCameraFrame returned nullptr: " << SDL_GetError() << '\n';
            loggedAcquireFailure = true;
        }

        return;
    }

    if (!loggedFirstFrame)
    {
        std::cout << "First camera frame acquired: "
            << frame->w << "x" << frame->h
            << ", format: " << SDL_GetPixelFormatName(frame->format)
            << '\n';
        loggedFirstFrame = true;
    }

    SDL_Surface* convertedFrame = nullptr;
    SDL_Surface* sourceFrame = frame;

    if (frame->format != SDL_PIXELFORMAT_RGBA32)
    {
        convertedFrame = SDL_ConvertSurface(frame, SDL_PIXELFORMAT_RGBA32);

        if (convertedFrame)
            sourceFrame = convertedFrame;
    }

    if (sourceFrame->pixels)
    {
        const int byteCount = sourceFrame->pitch * sourceFrame->h;

        m_frame.width = sourceFrame->w;
        m_frame.height = sourceFrame->h;
        m_frame.pitch = sourceFrame->pitch;
        m_frame.pixels.resize(static_cast<std::size_t>(byteCount));
        std::memcpy(m_frame.pixels.data(), sourceFrame->pixels, static_cast<std::size_t>(byteCount));
        ++m_frame.version;
    }

    if (convertedFrame)
        SDL_DestroySurface(convertedFrame);

    SDL_ReleaseCameraFrame(m_camera, frame);
}

const ImageFrame& VideoSource::frame() const
{
    return m_frame;
}

bool VideoSource::isOpen() const
{
    return m_camera != nullptr;
}
