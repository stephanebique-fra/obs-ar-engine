#include "Projector.hpp"
#include "Homography.hpp"

Projector::Projector(const Homography& homography)
    : m_homography(homography)
{
}

cv::Point2f Projector::project(float x, float y) const
{
    return m_homography.courtToImage({x, y});
}

std::vector<cv::Point2f> Projector::project(
    const std::vector<cv::Point2f>& points) const
{
    std::vector<cv::Point2f> result;
    result.reserve(points.size());

    for (const auto& point : points)
    {
        result.push_back(m_homography.courtToImage(point));
    }

    return result;
}