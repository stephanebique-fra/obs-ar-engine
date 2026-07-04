#include "Court.hpp"

#include <cmath>

namespace
{
constexpr float Pi = 3.14159265358979323846f;

constexpr float CourtLength = 28.0f;
constexpr float CourtWidth = 15.0f;
constexpr float HalfCourtLength = CourtLength * 0.5f;
constexpr float HalfCourtWidth = CourtWidth * 0.5f;

constexpr float CenterCircleRadius = 1.8f;
constexpr float FreeThrowCircleRadius = 1.8f;
constexpr float FreeThrowLineDistance = 5.8f;
constexpr float LaneWidth = 4.9f;
constexpr float BasketOffset = 1.575f;
constexpr float BasketRadius = 0.225f;
constexpr float ThreePointRadius = 6.75f;
constexpr float ThreePointSidelineDistance = 0.9f;

Court::Point leftBasket()
{
    return { BasketOffset, HalfCourtWidth };
}

Court::Point rightBasket()
{
    return { CourtLength - BasketOffset, HalfCourtWidth };
}

float threePointIntersectionOffset()
{
    const float dy = HalfCourtWidth - ThreePointSidelineDistance;
    return std::sqrt((ThreePointRadius * ThreePointRadius) - (dy * dy));
}
}

float Court::length() const
{
    return CourtLength;
}

float Court::width() const
{
    return CourtWidth;
}

std::array<Court::Line, Court::LineCount> Court::lines() const
{
    const float laneTop = HalfCourtWidth - (LaneWidth * 0.5f);
    const float laneBottom = HalfCourtWidth + (LaneWidth * 0.5f);
    const float threePointTop = ThreePointSidelineDistance;
    const float threePointBottom = CourtWidth - ThreePointSidelineDistance;
    const float arcOffset = threePointIntersectionOffset();
    const float leftArcX = BasketOffset + arcOffset;
    const float rightArcX = CourtLength - BasketOffset - arcOffset;

    return {
        Line{ { 0.0f, 0.0f }, { CourtLength, 0.0f } },
        Line{ { CourtLength, 0.0f }, { CourtLength, CourtWidth } },
        Line{ { CourtLength, CourtWidth }, { 0.0f, CourtWidth } },
        Line{ { 0.0f, CourtWidth }, { 0.0f, 0.0f } },

        Line{ { HalfCourtLength, 0.0f }, { HalfCourtLength, CourtWidth } },

        Line{ { 0.0f, laneTop }, { FreeThrowLineDistance, laneTop } },
        Line{ { FreeThrowLineDistance, laneTop }, { FreeThrowLineDistance, laneBottom } },
        Line{ { FreeThrowLineDistance, laneBottom }, { 0.0f, laneBottom } },

        Line{ { CourtLength, laneTop }, { CourtLength - FreeThrowLineDistance, laneTop } },
        Line{ { CourtLength - FreeThrowLineDistance, laneTop }, { CourtLength - FreeThrowLineDistance, laneBottom } },
        Line{ { CourtLength - FreeThrowLineDistance, laneBottom }, { CourtLength, laneBottom } },

        Line{ { 0.0f, threePointTop }, { leftArcX, threePointTop } },
        Line{ { 0.0f, threePointBottom }, { leftArcX, threePointBottom } },
        Line{ { CourtLength, threePointTop }, { rightArcX, threePointTop } },
        Line{ { CourtLength, threePointBottom }, { rightArcX, threePointBottom } },
    };
}

std::array<Court::Circle, Court::CircleCount> Court::circles() const
{
    return {
        Circle{ { HalfCourtLength, HalfCourtWidth }, CenterCircleRadius },
        Circle{ leftBasket(), BasketRadius },
        Circle{ rightBasket(), BasketRadius },
    };
}

std::array<Court::Arc, Court::ArcCount> Court::arcs() const
{
    const float dy = HalfCourtWidth - ThreePointSidelineDistance;
    const float threePointAngle = std::atan2(dy, threePointIntersectionOffset());

    return {
        Arc{ { FreeThrowLineDistance, HalfCourtWidth }, FreeThrowCircleRadius, -Pi * 0.5f, Pi * 0.5f },
        Arc{ { CourtLength - FreeThrowLineDistance, HalfCourtWidth }, FreeThrowCircleRadius, Pi * 0.5f, Pi * 1.5f },
        Arc{ leftBasket(), ThreePointRadius, -threePointAngle, threePointAngle },
        Arc{ rightBasket(), ThreePointRadius, Pi - threePointAngle, Pi + threePointAngle },
    };
}
