#include "Calibration.hpp"

#include <cassert>

void Calibration::setImagePoint(std::size_t index, float x, float y)
{
    setImagePoint(index, { x, y });
}

void Calibration::setImagePoint(std::size_t index, ImagePoint point)
{
    assert(index < PointCount);
    m_imagePoints[index] = point;
}

void Calibration::moveImagePoint(std::size_t index, float dx, float dy)
{
    assert(index < PointCount);

    m_imagePoints[index].x += dx;
    m_imagePoints[index].y += dy;
}

const Calibration::ImagePoint& Calibration::imagePoint(std::size_t index) const
{
    assert(index < PointCount);
    return m_imagePoints[index];
}

const std::array<Calibration::ImagePoint, Calibration::PointCount>& Calibration::imagePoints() const
{
    return m_imagePoints;
}

#include <limits>
#include <cmath>

std::size_t Calibration::findNearestImagePoint(
    float x,
    float y,
    float maxDistance) const
{
    std::size_t nearest = PointCount;
    float bestDistanceSquared = maxDistance * maxDistance;

    for (std::size_t i = 0; i < PointCount; ++i)
    {
        const float dx = m_imagePoints[i].x - x;
        const float dy = m_imagePoints[i].y - y;
        const float distanceSquared = dx * dx + dy * dy;

        if (distanceSquared <= bestDistanceSquared)
        {
            bestDistanceSquared = distanceSquared;
            nearest = i;
        }
    }

    return nearest;
}