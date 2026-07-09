#pragma once

#include "Court.hpp"
#include "FibaMarker.hpp"

class FibaCourt
{
public:
    static Court::Point markerPosition(FibaMarker marker);
};