#pragma once

#include "Court.hpp"

#include <SDL3/SDL.h>

class Camera2D;

class Renderer
{
public:
    Renderer() = default;

    bool initialize(SDL_Renderer* renderer);

    void setLineThickness(float metres);

    void clear();
    void present();

    void draw(
        const Court& court,
        const Camera2D& camera);

private:
    struct ScreenPoint
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    ScreenPoint toScreen(
        const Court& court,
        const Camera2D& camera,
        float metresToPixels,
        const Court::Point& point) const;

    void drawLine(
        const ScreenPoint& start,
        const ScreenPoint& end,
        float thicknessPixels);

    void drawCircle(
        const Court& court,
        const Camera2D& camera,
        float metresToPixels,
        const Court::Circle& circle);

    void drawArc(
        const Court& court,
        const Camera2D& camera,
        float metresToPixels,
        const Court::Arc& arc);

    SDL_Renderer* m_renderer = nullptr;
    float m_lineThicknessMetres = 0.05f;
};
