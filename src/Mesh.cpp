#include "Mesh.hpp"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    destroy();
}

bool Mesh::createQuad()
{
    const float vertices[] =
    {
        // Position          // UV
    -0.5f,  0.5f, 0.0f,   0.0f, 0.0f,
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f,
     0.5f, -0.5f, 0.0f,   1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 1.0f,
    };

    const unsigned int indices[] =
    {
        0,1,2,
        2,3,0
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);

    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        m_ebo);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return true;
}

void Mesh::destroy()
{
    if (m_ebo)
    {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }

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
}

void Mesh::draw() const
{
    glBindVertexArray(m_vao);

    glDrawElements(
        GL_TRIANGLES,
        6,
        GL_UNSIGNED_INT,
        nullptr);

    glBindVertexArray(0);
}