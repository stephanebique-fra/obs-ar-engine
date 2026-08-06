#pragma once

#include "Shader.hpp"
#include "Mesh.hpp"
#include "TextureGL.hpp"
#include "ImageFrame.hpp"
#include "Homography.hpp"
#include "LineMesh.hpp"
#include "Court.hpp"
#include <vector>
#include "ProjectedQuad.hpp"

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
    void drawProjectedQuad(
        const ProjectedQuad &quad,
        const Homography &homography);

    void drawProjectedTexture(
        float x,
        float y,
        float width,
        float height,
        const Homography &homography);
    void drawProjectedCourt(
        const Court &court,
        const Homography &homography);
    void drawTestTriangle();

private:
    bool createTriangle();
    TextureGL m_backgroundTexture;

    bool m_backgroundCreated = false;

private:
    Shader m_shader;
    Shader m_colorShader;
    Mesh m_screenMesh;
    Mesh m_projectedMesh;
    TextureGL m_texture;
    LineMesh m_lineMesh;
    float m_viewportWidth = 1280.0f;
    float m_viewportHeight = 720.0f;
    void appendProjectedArc(
        std::vector<float> &vertices,
        const Court::Arc &arc,
        const Homography &homography);
    float toNdcX(float x) const;
    float toNdcY(float y) const;
    std::array<Vertex, 4> buildProjectedQuad(
        float x,
        float y,
        float width,
        float height,
        const Homography &homography) const;
    cv::Point2f projectToNdc(
        const cv::Point2f &imagePoint) const;
};
