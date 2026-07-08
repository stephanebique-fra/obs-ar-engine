#include "Homography.hpp"
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

bool Homography::compute(
    const std::vector<cv::Point2f>& imagePoints,
    const std::vector<cv::Point2f>& courtPoints)
{
    if (imagePoints.size() < 4 ||
        courtPoints.size() < 4 ||
        imagePoints.size() != courtPoints.size())
    {
        m_imageToCourt.release();
        m_courtToImage.release();
        return false;
    }

    m_imageToCourt = cv::findHomography(
        imagePoints,
        courtPoints,
        cv::RANSAC);

    if (m_imageToCourt.empty())
    {
        m_courtToImage.release();
        return false;
    }

    m_courtToImage = m_imageToCourt.inv();

    return true;
}

void Homography::reset()
{
    m_imageToCourt.release();
    m_courtToImage.release();
}

cv::Point2f Homography::transform(
    const cv::Mat& matrix,
    const cv::Point2f& point) const
{
    std::vector<cv::Point2f> input{ point };
    std::vector<cv::Point2f> output;

    cv::perspectiveTransform(input, output, matrix);

    return output.front();
}

bool Homography::isValid() const
{
    return !m_imageToCourt.empty() && !m_courtToImage.empty();
}

cv::Point2f Homography::imageToCourt(const cv::Point2f& point) const
{
    if (m_imageToCourt.empty())
        return {};

    return transform(m_imageToCourt, point);
}

cv::Point2f Homography::courtToImage(const cv::Point2f& point) const
{
    if (m_courtToImage.empty())
        return {};

    return transform(m_courtToImage, point);
}