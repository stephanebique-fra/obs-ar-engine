#pragma once

#include "Transform2D.hpp"

struct Advertisement
{
    float x;
    float y;

    float width;
    float height;

    int textureId = 0;

    bool visible;
};