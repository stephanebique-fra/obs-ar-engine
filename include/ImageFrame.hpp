#pragma once

#include <vector>

struct ImageFrame
{
    int width = 0;
    int height = 0;
    int pitch = 0;
    unsigned int version = 0;
    std::vector<unsigned char> pixels;

    bool isValid() const
    {
        return width > 0 && height > 0 && pitch > 0 && !pixels.empty();
    }
};
