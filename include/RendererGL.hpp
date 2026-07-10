#pragma once

#include "Shader.hpp"
#include "Mesh.hpp"
#include "TextureGL.hpp"

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

    void drawQuad();

private:
    bool createTriangle();

private:
    Shader m_shader;
    Mesh m_mesh;
    TextureGL m_texture;
};