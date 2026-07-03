#include "graphics/Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

bool Image::load(const std::string& filename)
{
    unsigned char* data = stbi_load(
        filename.c_str(),
        &m_width,
        &m_height,
        &m_channels,
        4);

    if (!data)
        return false;

    m_channels = 4;

    m_pixels.assign(
        data,
        data + (m_width * m_height * m_channels));

    stbi_image_free(data);

    return true;
}

int Image::width() const
{
    return m_width;
}

int Image::height() const
{
    return m_height;
}

int Image::channels() const
{
    return m_channels;
}

const unsigned char* Image::pixels() const
{
    return m_pixels.data();
}
