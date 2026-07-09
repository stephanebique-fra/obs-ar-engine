#include "Renderer.hpp"
#include "Camera2D.hpp"
#include "Court.hpp"
#include "ImageFrame.hpp"
#include "Calibration.hpp"
#include "Homography.hpp"
#include <opencv2/core.hpp>

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

Renderer::~Renderer()
{
    destroy();
}

void Renderer::destroy()
{
    destroyBackgroundTexture();
    m_renderer = nullptr;
}

void Renderer::setLineThickness(float metres)
{
    m_lineThicknessMetres = std::max(0.0f, metres);
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

void Renderer::drawBackground(const ImageFrame& frame)
{
    updateBackgroundTexture(frame);

    if (!m_backgroundTexture)
        return;

    int windowWidth = 0;
    int windowHeight = 0;

    SDL_GetRenderOutputSize(
        m_renderer,
        &windowWidth,
        &windowHeight);

    if (m_backgroundWidth <= 0.0f || m_backgroundHeight <= 0.0f)
        return;

    const float scale = std::max(
        static_cast<float>(windowWidth) / m_backgroundWidth,
        static_cast<float>(windowHeight) / m_backgroundHeight);
    const float width = m_backgroundWidth * scale;
    const float height = m_backgroundHeight * scale;

    const SDL_FRect destination =
    {
        (static_cast<float>(windowWidth) - width) * 0.5f,
        (static_cast<float>(windowHeight) - height) * 0.5f,
        width,
        height
    };

    SDL_RenderTexture(
        m_renderer,
        m_backgroundTexture,
        nullptr,
        &destination);
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
    const float thicknessPixels = std::max(1.0f, m_lineThicknessMetres * metresToPixels);

    SDL_SetRenderDrawColor(m_renderer, 235, 235, 225, 255);

    for (const Court::Line& line : court.lines())
    {
        const ScreenPoint start = toScreen(court, camera, metresToPixels, line.start);
        const ScreenPoint end = toScreen(court, camera, metresToPixels, line.end);

        drawLine(start, end, thicknessPixels);
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

void Renderer::drawLine(
    const ScreenPoint& start,
    const ScreenPoint& end,
    float thicknessPixels)
{
    const float dx = end.x - start.x;
    const float dy = end.y - start.y;
    const float length = std::sqrt((dx * dx) + (dy * dy));

    if (length <= 0.0f)
        return;

    const float normalX = -dy / length;
    const float normalY = dx / length;
    const int lineCount = static_cast<int>(std::ceil(thicknessPixels));
    const float startOffset = (static_cast<float>(lineCount) - 1.0f) * -0.5f;

    for (int i = 0; i < lineCount; ++i)
    {
        const float offset = startOffset + static_cast<float>(i);

        SDL_RenderLine(
            m_renderer,
            start.x + (normalX * offset),
            start.y + (normalY * offset),
            end.x + (normalX * offset),
            end.y + (normalY * offset));
    }
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
    const float thicknessPixels = std::max(1.0f, m_lineThicknessMetres * metresToPixels);
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
            drawLine(previous, current, thicknessPixels);

        previous = current;
    }
}

void Renderer::updateBackgroundTexture(const ImageFrame& frame)
{
    const unsigned int frameVersion = frame.version;

    if (frameVersion == m_backgroundFrameVersion)
        return;

    if (!frame.isValid())
    {
        destroyBackgroundTexture();
        m_backgroundFrameVersion = frameVersion;
        return;
    }

    if (!m_backgroundTexture
        || m_backgroundWidth != static_cast<float>(frame.width)
        || m_backgroundHeight != static_cast<float>(frame.height))
    {
        destroyBackgroundTexture();

        m_backgroundTexture = SDL_CreateTexture(
            m_renderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            frame.width,
            frame.height);

        if (!m_backgroundTexture)
            return;

        SDL_SetTextureScaleMode(m_backgroundTexture, SDL_SCALEMODE_LINEAR);
        m_backgroundWidth = static_cast<float>(frame.width);
        m_backgroundHeight = static_cast<float>(frame.height);
    }

    if (!SDL_UpdateTexture(
        m_backgroundTexture,
        nullptr,
        frame.pixels.data(),
        frame.pitch))
        return;

    m_backgroundFrameVersion = frameVersion;
}

void Renderer::destroyBackgroundTexture()
{
    if (m_backgroundTexture)
    {
        SDL_DestroyTexture(m_backgroundTexture);
        m_backgroundTexture = nullptr;
    }

    m_backgroundWidth = 0.0f;
    m_backgroundHeight = 0.0f;
}
void Renderer::drawCalibration(
    const Calibration& calibration,
    std::size_t selectedPoint,
    float pointSize)
{
    for (std::size_t i = 0; i < calibration.pointCount(); ++i)
    {
        const auto& point = calibration.point(i);

        if (!point.enabled)
        {
            // Gris = désactivé
            SDL_SetRenderDrawColor(m_renderer, 120, 120, 120, 255);
        }
        else if (i == selectedPoint)
        {
            // Jaune = sélectionné
            SDL_SetRenderDrawColor(m_renderer, 255, 255, 0, 255);
        }
        else
        {
            // Vert = actif
            SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
        }

        const float size =
            (i == selectedPoint)
            ? pointSize * 1.5f
            : pointSize;

        SDL_FRect rect =
        {
            point.imageX - size * 0.5f,
            point.imageY - size * 0.5f,
            size,
            size
        };

        SDL_RenderFillRect(m_renderer, &rect);

        // Contour noir pour améliorer la visibilité
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderRect(m_renderer, &rect);
    }
}
void Renderer::drawMarker(float x, float y)
{
    SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);

    SDL_FRect marker =
    {
        x - 5.0f,
        y - 5.0f,
        10.0f,
        10.0f
    };

    SDL_RenderFillRect(m_renderer, &marker);
}
void Renderer::drawProjectedCourt(
    const Court& court,
    const Homography& homography)
{
    if (!homography.isValid())
        return;

    SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);

    for (const Court::Line& line : court.lines())
    {
        cv::Point2f p1 =
            homography.courtToImage(
                { line.start.x, line.start.y });

        cv::Point2f p2 =
            homography.courtToImage(
                { line.end.x, line.end.y });

        SDL_RenderLine(
            m_renderer,
            p1.x,
            p1.y,
            p2.x,
            p2.y);
    }
    for (const Court::Circle& circle : court.circles())
    {
        drawProjectedArc(
            { circle.center, circle.radius, 0.0f, 6.28318530717958647692f },
            homography);
    }

    for (const Court::Arc& arc : court.arcs())
    {
        drawProjectedArc(
            arc,
            homography);
    }
}
void Renderer::drawProjectedArc(
    const Court::Arc& arc,
    const Homography& homography)
{
    constexpr int Segments = 64;

    cv::Point2f previous;

    for (int i = 0; i <= Segments; ++i)
    {
        const float t = static_cast<float>(i) / static_cast<float>(Segments);

        const float angle =
            arc.startRadians +
            ((arc.endRadians - arc.startRadians) * t);

        const Court::Point point =
        {
            arc.center.x + std::cos(angle) * arc.radius,
            arc.center.y + std::sin(angle) * arc.radius
        };

        const cv::Point2f current =
            homography.courtToImage(
                { point.x, point.y });

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