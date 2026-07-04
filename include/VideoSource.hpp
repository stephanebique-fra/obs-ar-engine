#pragma once

#include "ImageFrame.hpp"

struct SDL_Camera;

class VideoSource
{
public:
    VideoSource() = default;
    ~VideoSource();

    bool openDefaultCamera();
    void close();

    void update();

    const ImageFrame& frame() const;
    bool isOpen() const;

private:
    SDL_Camera* m_camera = nullptr;
    ImageFrame m_frame;
};
