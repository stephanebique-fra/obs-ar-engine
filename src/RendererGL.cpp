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

    return createTriangle();
}

void RendererGL::destroy()
{
    if (m_vbo)
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }

    if (m_vao)
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
}

bool RendererGL::createTriangle()
{
    const float vertices[] =
    {
         0.0f,  0.6f, 0.0f,
        -0.6f, -0.6f, 0.0f,
         0.6f, -0.6f, 0.0f
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return true;
}

void RendererGL::beginFrame()
{
    glViewport(0, 0, 1280, 720);

    glClearColor(
        0.1f,
        0.1f,
        0.1f,
        1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
}

void RendererGL::endFrame()
{
}

void RendererGL::drawTriangle()
{
    m_shader.use();

    glBindVertexArray(m_vao);

    glDrawArrays(
        GL_TRIANGLES,
        0,
        3);

    glBindVertexArray(0);
}