#pragma once

#include <SDL3/SDL.h>

class Camera2D
{
public:
    void reset();

    void setPosition(float x, float y);
    void move(float dx, float dy);

    void setZoom(float zoom);
    void zoom(float factor);

    float x() const;
    float y() const;
    float zoom() const;

private:
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_zoom = 1.0f;
};