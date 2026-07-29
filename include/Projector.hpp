#pragma once

#include <opencv2/core.hpp>
#include <vector>

class Homography;

class Projector
{
public:
    explicit Projector(const Homography& homography);

    cv::Point2f project(float x, float y) const;

    std::vector<cv::Point2f> project(
        const std::vector<cv::Point2f>& points) const;

private:
    const Homography& m_homography;
};