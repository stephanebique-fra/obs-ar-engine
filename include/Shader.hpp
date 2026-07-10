#pragma once

#include <string>

class Shader
{
public:
    Shader();
    ~Shader();

    bool load(const std::string& vertexSource,
              const std::string& fragmentSource);

    void use() const;

    unsigned int program() const;

private:
    bool compile(unsigned int shader,
                 const std::string& source);

private:
    unsigned int m_program = 0;
};