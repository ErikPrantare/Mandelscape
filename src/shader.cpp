#include <string>
#include <iostream>

#include "shader.h"
#include "utils.h"

void
Shader::loadFromFile(const std::string& filename, GLenum shaderType)
{
    loadFromCode(readFile(filename), shaderType);
}

void
Shader::loadFromCode(const std::string& code, GLenum shaderType)
{
    m_location = glCreateShader(shaderType);
    m_type     = shaderType;

    if(m_location == 0) {
        std::cerr << "Error creating shader type " << shaderType << std::endl;
        exit(1);
    }

    const GLchar* p[1];
    p[0] = code.c_str();
    GLint lengths[1];
    lengths[0] = code.size();
    glShaderSource(m_location, 1, p, lengths);
    glCompileShader(m_location);
    GLint success;
    glGetShaderiv(m_location, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(m_location, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Error compiling shader type " << shaderType << ": "
                  << "'" << infoLog << "'" << std::endl;

        exit(1);
    }
}
