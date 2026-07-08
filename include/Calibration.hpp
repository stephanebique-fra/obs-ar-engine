#pragma once

#include "CalibrationPoint.hpp"

#include <cstddef>
#include <string>
#include <vector>

class Calibration
{
public:
    void addPoint(const CalibrationPoint& point);

    void setPoint(std::size_t index, const CalibrationPoint& point);
    void movePoint(std::size_t index, float dx, float dy);

    const CalibrationPoint& point(std::size_t index) const;
    std::size_t pointCount() const;

    std::size_t findNearestImagePoint(
        float x,
        float y,
        float maxDistance) const;

    bool save(const std::string& filename) const;
    bool load(const std::string& filename);

private:
    std::vector<CalibrationPoint> m_points;
};