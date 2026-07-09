#pragma once

#include "CalibrationPoint.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <opencv2/core.hpp>

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
    void togglePoint(std::size_t index);
    std::vector<cv::Point2f> imagePoints() const;
    std::vector<cv::Point2f> courtPoints() const;
    void addPoint(float imageX, float imageY);
    void removePoint(std::size_t index);
    void centerPoint(std::size_t index, float x, float y);

private:
    std::vector<CalibrationPoint> m_points;
};
