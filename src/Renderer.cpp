#include "Renderer.hpp"
#include "Camera2D.hpp"
#include "Court.hpp"

#include <algorithm>
#include <cmath>

namespace
{
constexpr int CircleSegments = 96;
constexpr int ArcSegments = 64;
constexpr float CourtFit = 0.9f;
}

bool Renderer::initialize(SDL_Renderer* renderer)
{
    m_renderer = renderer;
    return m_renderer != nullptr;
}

void Renderer::clear()
{
    SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
    SDL_RenderClear(m_renderer);
}

void Renderer::present()
{
    SDL_RenderPresent(m_renderer);
}

void Renderer::draw(
    const Court& court,
    const Camera2D& camera)
{
    int windowWidth = 0;
    int windowHeight = 0;

    SDL_GetRenderOutputSize(
        m_renderer,
        &windowWidth,
        &windowHeight);

    const float scaleX = static_cast<float>(windowWidth) / court.length();
    const float scaleY = static_cast<float>(windowHeight) / court.width();
    const float metresToPixels = std::min(scaleX, scaleY) * CourtFit * camera.zoom();

    SDL_SetRenderDrawColor(m_renderer, 235, 235, 225, 255);

    for (const Court::Line& line : court.lines())
    {
        const ScreenPoint start = toScreen(court, camera, metresToPixels, line.start);
        const ScreenPoint end = toScreen(court, camera, metresToPixels, line.end);

        SDL_RenderLine(
            m_renderer,
            start.x,
            start.y,
            end.x,
            end.y);
    }

    for (const Court::Circle& circle : court.circles())
        drawCircle(court, camera, metresToPixels, circle);

    for (const Court::Arc& arc : court.arcs())
        drawArc(court, camera, metresToPixels, arc);
}

Renderer::ScreenPoint Renderer::toScreen(
    const Court& court,
    const Camera2D& camera,
    float metresToPixels,
    const Court::Point& point) const
{
    int windowWidth = 0;
    int windowHeight = 0;

    SDL_GetRenderOutputSize(
        m_renderer,
        &windowWidth,
        &windowHeight);

    const float courtWidthPixels = court.length() * metresToPixels;
    const float courtHeightPixels = court.width() * metresToPixels;
    const float originX = (static_cast<float>(windowWidth) - courtWidthPixels) * 0.5f + camera.x();
    const float originY = (static_cast<float>(windowHeight) - courtHeightPixels) * 0.5f + camera.y();

    return {
        originX + (point.x * metresToPixels),
        originY + (point.y * metresToPixels)
    };
}

void Renderer::drawCircle(
    const Court& court,
    const Camera2D& camera,
    float metresToPixels,
    const Court::Circle& circle)
{
    drawArc(
        court,
        camera,
        metresToPixels,
        { circle.center, circle.radius, 0.0f, 6.28318530717958647692f });
}

void Renderer::drawArc(
    const Court& court,
    const Camera2D& camera,
    float metresToPixels,
    const Court::Arc& arc)
{
    const int segments = std::abs(arc.endRadians - arc.startRadians) > 6.0f
        ? CircleSegments
        : ArcSegments;

    ScreenPoint previous;

    for (int i = 0; i <= segments; ++i)
    {
        const float t = static_cast<float>(i) / static_cast<float>(segments);
        const float angle = arc.startRadians + ((arc.endRadians - arc.startRadians) * t);
        const Court::Point point =
        {
            arc.center.x + (std::cos(angle) * arc.radius),
            arc.center.y + (std::sin(angle) * arc.radius)
        };
        const ScreenPoint current = toScreen(court, camera, metresToPixels, point);

        if (i > 0)
        {
            SDL_RenderLine(
                m_renderer,
                previous.x,
                previous.y,
                current.x,
                current.y);
        }

        previous = current;
    }
}
