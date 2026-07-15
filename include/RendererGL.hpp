#pragma once

#include "Shader.hpp"
#include "Mesh.hpp"
#include "TextureGL.hpp"
#include "ImageFrame.hpp"
#include "Homography.hpp"
#include "LineMesh.hpp"

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
    void drawTexture(const TextureGL &texture);
    void drawBackground(const ImageFrame &frame);
    void drawProjectedRectangle(
        float x,
        float y,
        float width,
        float height,
        const Homography &homography);

private:
    bool createTriangle();
    TextureGL m_backgroundTexture;

    bool m_backgroundCreated = false;

private:
    Shader m_shader;
    Shader m_colorShader;
    Mesh m_mesh;
    TextureGL m_texture;
    LineMesh m_lineMesh;
    float toNdcX(float x) const;
    float toNdcY(float y) const;
};