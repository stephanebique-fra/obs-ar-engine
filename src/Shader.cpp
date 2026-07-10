#include "Shader.hpp"

#include <glad/gl.h>

#include <iostream>

Shader::Shader()
{
}

Shader::~Shader()
{
    if (m_program)
        glDeleteProgram(m_program);
}

bool Shader::compile(
    unsigned int shader,
    const std::string& source)
{
    const char* src = source.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;

    glGetShaderiv(
        shader,
        GL_COMPILE_STATUS,
        &success);

    if (!success)
    {
        char log[1024];

        glGetShaderInfoLog(
            shader,
            sizeof(log),
            nullptr,
            log);

        std::cout << log << std::endl;

        return false;
    }

    return true;
}

bool Shader::load(
    const std::string& vertexSource,
    const std::string& fragmentSource)
{
    GLuint vertex =
        glCreateShader(GL_VERTEX_SHADER);

    GLuint fragment =
        glCreateShader(GL_FRAGMENT_SHADER);

    if (!compile(vertex, vertexSource))
        return false;

    if (!compile(fragment, fragmentSource))
        return false;

    m_program = glCreateProgram();

    glAttachShader(
        m_program,
        vertex);

    glAttachShader(
        m_program,
        fragment);

    glLinkProgram(m_program);

    GLint success = GL_FALSE;

    glGetProgramiv(
        m_program,
        GL_LINK_STATUS,
        &success);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if (!success)
    {
        char log[1024];

        glGetProgramInfoLog(
            m_program,
            sizeof(log),
            nullptr,
            log);

        std::cout << log << std::endl;

        return false;
    }

    return true;
}

void Shader::use() const
{
    glUseProgram(m_program);
}

unsigned int Shader::program() const
{
    return m_program;
}

void Shader::setInt(
    const std::string& name,
    int value) const
{
    GLint location =
        glGetUniformLocation(
            m_program,
            name.c_str());

    if (location >= 0)
        glUniform1i(location, value);
}