#include "LineMesh.hpp"
#include <iostream>

LineMesh::LineMesh()
{
}

LineMesh::~LineMesh()
{
    destroy();
}

bool LineMesh::initialize()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        0,
        nullptr,
        GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        nullptr);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return true;
}

void LineMesh::destroy()
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

    m_vertexCount = 0;
}

void LineMesh::update(
    const std::vector<float> &vertices)
{
    m_vertexCount =
        static_cast<GLsizei>(vertices.size() / 2);

    glBindVertexArray(m_vao);

    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}
void LineMesh::draw(GLenum mode) const
{
    if (m_vertexCount == 0)
        return;

    glBindVertexArray(m_vao);

    std::cout << "VertexCount = "
              << m_vertexCount
              << std::endl;

    glDrawArrays(
        mode,
        0,
        m_vertexCount);

    GLenum err = glGetError();

    std::cout << "glDrawArrays error = "
              << err
              << std::endl;

    glBindVertexArray(0);
}