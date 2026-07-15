#pragma once

#include <glad/gl.h>
#include <vector>

class LineMesh
{
public:
    LineMesh();
    ~LineMesh();

    bool initialize();
    void destroy();

    void update(const std::vector<float>& vertices);

    void draw(GLenum mode = GL_LINES) const;

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;

    GLsizei m_vertexCount = 0;
};