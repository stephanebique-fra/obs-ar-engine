#include "RendererGL.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
std::string loadTextFile(const std::string& filename)
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

    if (!m_texture.load("assets/images/logo.png"))
        return false;
    m_shader.use();
    m_shader.setInt("uTexture", 0);
    return m_mesh.createQuad();
}

void RendererGL::destroy()
{
    m_texture.destroy();
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