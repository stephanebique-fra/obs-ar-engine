#include "RendererGL.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
    std::string loadTextFile(const std::string &filename)
    {
        std::ifstream file(filename);

        if (!file)
            return {};

        std::stringstream stream;
        stream << file.rdbuf();

        return stream.str();
    }
}

RendererGL::RendererGL()
{
}

RendererGL::~RendererGL()
{
    destroy();
}

bool RendererGL::initialize()
{
    const std::string vertex =
        loadTextFile("assets/shaders/basic.vert");

    const std::string fragment =
        loadTextFile("assets/shaders/basic.frag");

    if (vertex.empty() || fragment.empty())
    {
        std::cout << "Unable to load shaders\n";
        return false;
    }

    if (!m_shader.load(vertex, fragment))
        return false;

    const std::string colorVertex =
        loadTextFile("assets/shaders/color.vert");

    const std::string colorFragment =
        loadTextFile("assets/shaders/color.frag");

    if (colorVertex.empty() || colorFragment.empty())
    {
        std::cout << "Unable to load color shaders\n";
        return false;
    }

    if (!m_colorShader.load(colorVertex, colorFragment))
        return false;

    if (!m_texture.load("assets/images/logo.png"))
        return false;

    m_shader.use();
    m_shader.setInt("uTexture", 0);

    if (!m_mesh.createQuad())
        return false;

    if (!m_lineMesh.initialize())
        return false;

    return true;
}

void RendererGL::destroy()
{
    m_texture.destroy();
    m_lineMesh.destroy();
    m_mesh.destroy();
}

void RendererGL::beginFrame()
{
    glViewport(0, 0, 1280, 720);

    glClearColor(
        0.1f,
        0.1f,
        0.1f,
        1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RendererGL::endFrame()
{
}

void RendererGL::drawQuad()
{
    m_shader.use();

    m_texture.bind(0);

    m_mesh.draw();
}
void RendererGL::drawTexture(const TextureGL &texture)
{
    m_shader.use();

    texture.bind(0);

    m_mesh.draw();
}
void RendererGL::drawBackground(const ImageFrame &frame)
{
    if (!frame.isValid())
        return;

    if (!m_backgroundCreated)
    {
        m_backgroundTexture.create(
            frame.width,
            frame.height,
            GL_RGBA);

        m_backgroundCreated = true;
    }

    m_backgroundTexture.update(
        frame.pixels.data());

    drawTexture(m_backgroundTexture);
}
float RendererGL::toNdcX(float x) const
{
    return (2.0f * x / 1280.0f) - 1.0f;
}

float RendererGL::toNdcY(float y) const
{
    return 1.0f - (2.0f * y / 720.0f);
}
void RendererGL::drawProjectedRectangle(
    float x,
    float y,
    float width,
    float height,
    const Homography& homography)
{
    if (!homography.isValid())
        return;

    const cv::Point2f p1 =
        homography.courtToImage({x, y});

    const cv::Point2f p2 =
        homography.courtToImage({x + width, y});

    const cv::Point2f p3 =
        homography.courtToImage({x + width, y + height});

    const cv::Point2f p4 =
        homography.courtToImage({x, y + height});

    std::vector<float> vertices =
    {
        toNdcX(p1.x), toNdcY(p1.y),
        toNdcX(p2.x), toNdcY(p2.y),

        toNdcX(p2.x), toNdcY(p2.y),
        toNdcX(p3.x), toNdcY(p3.y),

        toNdcX(p3.x), toNdcY(p3.y),
        toNdcX(p4.x), toNdcY(p4.y),

        toNdcX(p4.x), toNdcY(p4.y),
        toNdcX(p1.x), toNdcY(p1.y),

        toNdcX(p1.x), toNdcY(p1.y),
        toNdcX(p3.x), toNdcY(p3.y),

        toNdcX(p2.x), toNdcY(p2.y),
        toNdcX(p4.x), toNdcY(p4.y)
    };

    m_lineMesh.update(vertices);

    m_colorShader.use();

    m_colorShader.setVec4(
        "uColor",
        1.0f,
        0.0f,
        0.0f,
        1.0f);

    m_lineMesh.draw();
}