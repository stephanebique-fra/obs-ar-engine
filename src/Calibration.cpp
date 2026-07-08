#include "Calibration.hpp"

#include <cassert>
#include <cmath>
#include <fstream>
#include <string>

void Calibration::setPoint(std::size_t index, const CalibrationPoint& point)
{
    assert(index < m_points.size());
    m_points[index] = point;
}

void Calibration::movePoint(std::size_t index, float dx, float dy)
{
    assert(index < m_points.size());

    m_points[index].imageX += dx;
    m_points[index].imageY += dy;
}

std::size_t Calibration::findNearestImagePoint(
    float x,
    float y,
    float maxDistance) const
{
    std::size_t nearest = m_points.size();
    float bestDistanceSquared = maxDistance * maxDistance;

    for (std::size_t i = 0; i < m_points.size(); ++i)
    {
        const float dx = m_points[i].imageX - x;
        const float dy = m_points[i].imageY - y;
        const float distanceSquared = dx * dx + dy * dy;

        if (distanceSquared <= bestDistanceSquared)
        {
            bestDistanceSquared = distanceSquared;
            nearest = i;
        }
    }

    return nearest;
}

bool Calibration::save(const std::string& filename) const
{
    std::ofstream file(filename);

    if (!file)
        return false;

    for (std::size_t i = 0; i < m_points.size(); ++i)
    {
        file << "point"
             << i
             << "="
             << m_points[i].imageX
             << ","
             << m_points[i].imageY
             << ","
             << m_points[i].courtX
             << ","
             << m_points[i].courtY
             << "\n";
    }

    return true;
}

bool Calibration::load(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file)
        return false;

    m_points.clear();

    std::string line;

    while (std::getline(file, line))
    {
        std::size_t p1 = line.find('=');
        std::size_t p2 = line.find(',', p1 + 1);
        std::size_t p3 = line.find(',', p2 + 1);
        std::size_t p4 = line.find(',', p3 + 1);

        if (p1 == std::string::npos ||
            p2 == std::string::npos ||
            p3 == std::string::npos ||
            p4 == std::string::npos)
            continue;

        CalibrationPoint point;

        point.imageX = std::stof(line.substr(p1 + 1, p2 - p1 - 1));
        point.imageY = std::stof(line.substr(p2 + 1, p3 - p2 - 1));
        point.courtX = std::stof(line.substr(p3 + 1, p4 - p3 - 1));
        point.courtY = std::stof(line.substr(p4 + 1));

        m_points.push_back(point);
    }

    return true;
}

void Calibration::addPoint(const CalibrationPoint& point)
{
    m_points.push_back(point);
}

std::size_t Calibration::pointCount() const
{
    return m_points.size();
}

const CalibrationPoint& Calibration::point(std::size_t index) const
{
    assert(index < m_points.size());
    return m_points[index];
}