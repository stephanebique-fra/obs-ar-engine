#pragma once

#include <array>
#include <glad/gl.h>

struct Vertex
{
    float x;
    float y;
    float z;

    float u;
    float v;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool createQuad();

    void updateVertices(const float vertices[20]);

    void updateVertices(const std::array<Vertex, 4>& vertices);

    void destroy();

    void draw() const;

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
};