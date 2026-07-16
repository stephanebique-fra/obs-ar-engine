#pragma once

#include "Shader.hpp"
#include "Mesh.hpp"
#include "TextureGL.hpp"
#include "ImageFrame.hpp"
#include "Homography.hpp"
#include "LineMesh.hpp"
#include "Court.hpp"
#include <vector>

#include <glad/gl.h>

class RendererGL
{
public:
    RendererGL();
    ~RendererGL();

    bool initialize();
    void destroy();

    void beginFrame(int width, int height);
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
    void drawProjectedCourt(
    const Court& court,
    const Homography& homography);
    void drawTestTriangle();

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
    void appendProjectedArc(
        std::vector<float> &vertices,
        const Court::Arc &arc,
        const Homography &homography);
    float toNdcX(float x) const;
    float toNdcY(float y) const;
};
