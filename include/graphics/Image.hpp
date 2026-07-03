#pragma once

#include <string>
#include <vector>

class Image
{
public:
    bool load(const std::string& filename);

    int width() const;
    int height() const;
    int channels() const;

    const unsigned char* pixels() const;

private:
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;

    std::vector<unsigned char> m_pixels;
};