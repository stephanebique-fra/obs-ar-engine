#pragma once

#include <array>

class Court
{
public:
    struct Point
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct Line
    {
        Point start;
        Point end;
    };

    struct Circle
    {
        Point center;
        float radius = 0.0f;
    };

    struct Arc
    {
        Point center;
        float radius = 0.0f;
        float startRadians = 0.0f;
        float endRadians = 0.0f;
    };

    static constexpr int LineCount = 15;
    static constexpr int CircleCount = 3;
    static constexpr int ArcCount = 4;

    float length() const;
    float width() const;

    std::array<Line, LineCount> lines() const;
    std::array<Circle, CircleCount> circles() const;
    std::array<Arc, ArcCount> arcs() const;
};
