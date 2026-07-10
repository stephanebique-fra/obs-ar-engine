#pragma once

#include <glad/gl.h>

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool createQuad();
    void destroy();

    void draw() const;

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
};