#include "RendererGL.hpp"

#include <array>
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

    void printEnabledState(const char *name, GLenum capability)
    {
        std::cout << name << " = "
                  << (glIsEnabled(capability) == GL_TRUE)
                  << '\n';
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
    std::cout << "RendererGL::initialize()" << std::endl;
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

    if (!m_screenMesh.createQuad())
        return false;

    if (!m_projectedMesh.createQuad())
        return false;

    if (!m_lineMesh.initialize())
        return false;

    std::cout << "RendererGL initialized OK" << std::endl;
    return true;
}

void RendererGL::destroy()
{
    m_texture.destroy();

    m_backgroundTexture.destroy();

    m_screenMesh.destroy();
    m_projectedMesh.destroy();

    m_lineMesh.destroy();
}

void RendererGL::beginFrame(int width, int height)
{
    m_viewportWidth = static_cast<float>(width);
    m_viewportHeight = static_cast<float>(height);
    glViewport(0, 0, width, height);

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

    m_screenMesh.draw();
}
void RendererGL::drawTexture(const TextureGL &texture)
{
    m_shader.use();

    texture.bind(0);

    m_screenMesh.draw();
}

void RendererGL::drawProjectedTexture(
    float x,
    float y,
    float width,
    float height,
    const Homography &homography)
{
    ProjectedQuad quad{
        &m_texture,
        x,
        y,
        width,
        height};

    drawProjectedQuad(
        quad,
        homography);
}

void RendererGL::drawProjectedQuad(
    const ProjectedQuad &quad,
    const Homography &homography)
{
    if (!homography.isValid())
        return;

    if (quad.texture == nullptr)
        return;

    const auto vertices =
        buildProjectedQuad(
            quad.x,
            quad.y,
            quad.width,
            quad.height,
            homography);

    m_projectedMesh.updateVertices(vertices);

    m_shader.use();

    quad.texture->bind(0);

    m_projectedMesh.draw();
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
    return (2.0f * x / m_viewportWidth) - 1.0f;
}

float RendererGL::toNdcY(float y) const
{
    return 1.0f - (2.0f * y / m_viewportHeight);
}
cv::Point2f RendererGL::projectToNdc(
    const cv::Point2f &imagePoint) const
{
    return {
        toNdcX(imagePoint.x),
        toNdcY(imagePoint.y)};
}

std::array<Vertex, 4> RendererGL::buildProjectedQuad(
    float x,
    float y,
    float width,
    float height,
    const Homography &homography) const
{
    const cv::Point2f p1 =
        homography.courtToImage({x, y});

    const cv::Point2f p2 =
        homography.courtToImage({x + width, y});

    const cv::Point2f p3 =
        homography.courtToImage({x + width, y + height});

    const cv::Point2f p4 =
        homography.courtToImage({x, y + height});

    const auto n1 = projectToNdc(p1);
    const auto n2 = projectToNdc(p2);
    const auto n3 = projectToNdc(p3);
    const auto n4 = projectToNdc(p4);

    return {{{n1.x, n1.y, 0.0f, 0.0f, 0.0f},
             {n2.x, n2.y, 0.0f, 1.0f, 0.0f},
             {n3.x, n3.y, 0.0f, 1.0f, 1.0f},
             {n4.x, n4.y, 0.0f, 0.0f, 1.0f}}};
}

void RendererGL::drawProjectedRectangle(
    float x,
    float y,
    float width,
    float height,
    const Homography &homography)
{
    if (!homography.isValid())
        return;

    const auto quad =
        buildProjectedQuad(
            x,
            y,
            width,
            height,
            homography);

    std::vector<float> vertices =
        {
            quad[0].x, quad[0].y,
            quad[1].x, quad[1].y,

            quad[1].x, quad[1].y,
            quad[2].x, quad[2].y,

            quad[2].x, quad[2].y,
            quad[3].x, quad[3].y,

            quad[3].x, quad[3].y,
            quad[0].x, quad[0].y,

            quad[0].x, quad[0].y,
            quad[2].x, quad[2].y,

            quad[1].x, quad[1].y,
            quad[3].x, quad[3].y};

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
void RendererGL::appendProjectedArc(
    std::vector<float> &vertices,
    const Court::Arc &arc,
    const Homography &homography)
{
    constexpr int Segments = 64;

    for (int i = 0; i < Segments; ++i)
    {
        const float t1 =
            static_cast<float>(i) / Segments;

        const float t2 =
            static_cast<float>(i + 1) / Segments;

        const float a1 =
            arc.startRadians +
            (arc.endRadians - arc.startRadians) * t1;

        const float a2 =
            arc.startRadians +
            (arc.endRadians - arc.startRadians) * t2;

        Court::Point p1{
            arc.center.x + std::cos(a1) * arc.radius,
            arc.center.y + std::sin(a1) * arc.radius};

        Court::Point p2{
            arc.center.x + std::cos(a2) * arc.radius,
            arc.center.y + std::sin(a2) * arc.radius};

        cv::Point2f i1 =
            homography.courtToImage({p1.x, p1.y});

        cv::Point2f i2 =
            homography.courtToImage({p2.x, p2.y});

        const auto n1 = projectToNdc(i1);
        const auto n2 = projectToNdc(i2);

        vertices.push_back(n1.x);
        vertices.push_back(n1.y);

        vertices.push_back(n2.x);
        vertices.push_back(n2.y);
    }
}
void RendererGL::drawProjectedCourt(
    const Court &court,
    const Homography &homography)
{
    if (!homography.isValid())
        return;

    std::vector<float> vertices;

    for (const Court::Line &line : court.lines())
    {
        const cv::Point2f start =
            homography.courtToImage({line.start.x, line.start.y});

        const cv::Point2f end =
            homography.courtToImage({line.end.x, line.end.y});

        const auto startNdc = projectToNdc(start);
        const auto endNdc = projectToNdc(end);

        vertices.push_back(startNdc.x);
        vertices.push_back(startNdc.y);
        vertices.push_back(endNdc.x);
        vertices.push_back(endNdc.y);
    }

    for (const Court::Circle &circle : court.circles())
    {
        appendProjectedArc(
            vertices,
            {circle.center, circle.radius, 0.0f, 6.28318530717958647692f},
            homography);
    }

    for (const Court::Arc &arc : court.arcs())
        appendProjectedArc(vertices, arc, homography);

    m_lineMesh.update(vertices);

    m_colorShader.use();

    m_colorShader.setVec4(
        "uColor",
        0.0f,
        1.0f,
        0.0f,
        1.0f);

    m_lineMesh.draw(GL_LINES);
}
void RendererGL::drawTestTriangle()
{
    std::cout << "drawTestTriangle: entered\n";

    static GLuint vao = 0;
    static GLuint vbo = 0;

    if (vao == 0)
    {
        const float vertices[] =
            {
                0.0f, 0.7f,
                -0.6f, -0.6f,
                0.6f, -0.6f};

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(vertices),
            vertices,
            GL_STATIC_DRAW);

        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            2 * sizeof(float),
            nullptr);

        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    m_colorShader.use();

    GLint currentProgram = 0;
    GLint linked = GL_FALSE;
    GLint validated = GL_FALSE;

    glValidateProgram(m_colorShader.program());
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    glGetProgramiv(m_colorShader.program(), GL_LINK_STATUS, &linked);
    glGetProgramiv(m_colorShader.program(), GL_VALIDATE_STATUS, &validated);

    std::cout << "shader: expected=" << m_colorShader.program()
              << " current=" << currentProgram
              << " linked=" << linked
              << " validated=" << validated
              << '\n';

    m_colorShader.setVec4(
        "uColor",
        1.0f,
        1.0f,
        1.0f,
        1.0f);

    glBindVertexArray(vao);

    GLint boundVao = 0;
    GLint boundVbo = 0;
    GLint vboSize = 0;
    GLint attributeEnabled = GL_FALSE;
    GLint attributeSize = 0;
    GLint attributeType = 0;
    GLint attributeNormalized = GL_FALSE;
    GLint attributeStride = 0;
    GLint attributeBuffer = 0;
    void *attributePointer = nullptr;
    std::array<float, 6> uploadedVertices{};
    std::array<GLint, 4> viewport{};
    GLint drawFramebuffer = 0;
    GLint readFramebuffer = 0;
    GLint blendSource = 0;
    GLint blendDestination = 0;
    GLboolean colorMask[4]{};
    GLboolean depthMask = GL_FALSE;

    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundVbo);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vboSize);
    glGetBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        sizeof(uploadedVertices),
        uploadedVertices.data());
    glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &attributeEnabled);
    glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_SIZE, &attributeSize);
    glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_TYPE, &attributeType);
    glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &attributeNormalized);
    glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &attributeStride);
    glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &attributeBuffer);
    glGetVertexAttribPointerv(0, GL_VERTEX_ATTRIB_ARRAY_POINTER, &attributePointer);
    glGetIntegerv(GL_VIEWPORT, viewport.data());
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebuffer);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer);
    glGetIntegerv(GL_BLEND_SRC_RGB, &blendSource);
    glGetIntegerv(GL_BLEND_DST_RGB, &blendDestination);
    glGetBooleanv(GL_COLOR_WRITEMASK, colorMask);
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);

    std::cout << "vao: expected=" << vao
              << " bound=" << boundVao
              << " valid=" << (glIsVertexArray(vao) == GL_TRUE)
              << '\n';
    std::cout << "vbo: expected=" << vbo
              << " bound=" << boundVbo
              << " valid=" << (glIsBuffer(vbo) == GL_TRUE)
              << " size=" << vboSize
              << " vertices=";

    for (float vertex : uploadedVertices)
        std::cout << ' ' << vertex;

    std::cout << '\n';
    std::cout << "attribute[0]: enabled=" << attributeEnabled
              << " size=" << attributeSize
              << " type=" << attributeType
              << " normalized=" << attributeNormalized
              << " stride=" << attributeStride
              << " buffer=" << attributeBuffer
              << " pointer=" << attributePointer
              << '\n';
    std::cout << "viewport: " << viewport[0] << ',' << viewport[1]
              << ' ' << viewport[2] << 'x' << viewport[3] << '\n';
    std::cout << "framebuffer: draw=" << drawFramebuffer
              << " read=" << readFramebuffer << '\n';
    printEnabledState("state: cull-face", GL_CULL_FACE);
    printEnabledState("state: depth-test", GL_DEPTH_TEST);
    printEnabledState("state: scissor-test", GL_SCISSOR_TEST);
    printEnabledState("state: blend", GL_BLEND);
    std::cout << "state: blend-src=" << blendSource
              << " blend-dst=" << blendDestination
              << " color-mask=" << colorMask[0] << colorMask[1]
              << colorMask[2] << colorMask[3]
              << " depth-mask=" << depthMask
              << '\n';

    while (glGetError() != GL_NO_ERROR)
    {
    }

    std::cout << "glDrawArrays: mode=" << GL_TRIANGLES
              << " first=0 count=3\n";

    glDrawArrays(
        GL_TRIANGLES,
        0,
        3);

    const GLenum drawError = glGetError();
    std::array<GLubyte, 4> centerPixel{};

    glReadPixels(
        viewport[2] / 2,
        viewport[3] / 2,
        1,
        1,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        centerPixel.data());

    const GLenum readError = glGetError();

    std::cout << "glDrawArrays: error=" << drawError << '\n';
    std::cout << "framebuffer center pixel: "
              << static_cast<int>(centerPixel[0]) << ','
              << static_cast<int>(centerPixel[1]) << ','
              << static_cast<int>(centerPixel[2]) << ','
              << static_cast<int>(centerPixel[3])
              << " read-error=" << readError << '\n';

    glBindVertexArray(0);
}
