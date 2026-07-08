#pragma once

#include <array>
#include <cstddef>

class Calibration
{
public:
    struct ImagePoint
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    static constexpr std::size_t PointCount = 4;

    void setImagePoint(std::size_t index, float x, float y);
    void setImagePoint(std::size_t index, ImagePoint point);

    void moveImagePoint(std::size_t index, float dx, float dy);
    
    const ImagePoint& imagePoint(std::size_t index) const;
    const std::array<ImagePoint, PointCount>& imagePoints() const;
    std::size_t findNearestImagePoint(float x, float y, float maxDistance) const;

private:
    std::array<ImagePoint, PointCount> m_imagePoints = {};
};
