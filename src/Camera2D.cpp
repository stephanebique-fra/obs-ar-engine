#include "Camera2D.hpp"

void Camera2D::reset()
{
    m_x = 0.0f;
    m_y = 0.0f;
    m_zoom = 1.0f;
}

void Camera2D::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
}

void Camera2D::move(float dx, float dy)
{
    m_x += dx;
    m_y += dy;
}

void Camera2D::setZoom(float zoom)
{
    m_zoom = zoom;
}

void Camera2D::zoom(float factor)
{
    m_zoom *= factor;
}

float Camera2D::x() const
{
    return m_x;
}

float Camera2D::y() const
{
    return m_y;
}

float Camera2D::zoom() const
{
    return m_zoom;
}