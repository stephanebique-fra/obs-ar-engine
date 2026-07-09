#pragma once
#include "FibaMarker.hpp"

struct CalibrationPoint
{
    FibaMarker marker = FibaMarker::Unknown;
    float imageX = 0.0f;
    float imageY = 0.0f;

    float courtX = 0.0f;
    float courtY = 0.0f;

    bool enabled = true;
};