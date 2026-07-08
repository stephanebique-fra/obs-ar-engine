#pragma once

#include <vector>
#include <opencv2/core.hpp>

class Homography
{
public:
    bool compute(
        const std::vector<cv::Point2f>& imagePoints,
        const std::vector<cv::Point2f>& courtPoints);
    
    void reset();

    bool isValid() const;
    
    cv::Point2f transform(
    const cv::Mat& matrix,
    const cv::Point2f& point) const;
    
    cv::Point2f imageToCourt(const cv::Point2f& point) const;
    cv::Point2f courtToImage(const cv::Point2f& point) const;
    

private:
    cv::Mat m_imageToCourt;
    cv::Mat m_courtToImage;
};