#pragma once

#include "Shader.hpp"

#include <glad/gl.h>

class RendererGL
{
public:
    RendererGL();
    ~RendererGL();

    bool initialize();
    void destroy();

    void beginFrame();
    void endFrame();

    void drawTriangle();

private:
    bool createTriangle();

private:
    Shader m_shader;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
};