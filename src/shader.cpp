#include <string>
#include <iostream>

#include "shader.h"
#include "utils.h"

Shader::Shader(GLenum location, GLenum type)
{
    m_location = location;
    m_type     = type;
}

Shader::~Shader()
{
    glDeleteShader(m_location);
}

Shader
Shader::fromFile(const std::string& filename, GLenum shaderType)
{
    return Shader(fromCode(readFile(filename), shaderType));
}

Shader
Shader::fromCode(const std::string& code, GLenum shaderType)
{
    GLuint location = glCreateShader(shaderType);

    if(location == 0) {
        std::cerr << "Error creating shader type " << shaderType << std::endl;
        exit(1);
    }

    const GLchar* p[1];
    p[0] = code.c_str();
    GLint lengths[1];
    lengths[0] = code.size();
    glShaderSource(location, 1, p, lengths);
    glCompileShader(location);
    GLint success;
    glGetShaderiv(location, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(location, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Error compiling shader type " << shaderType << ": "
                  << "'" << infoLog << "'" << std::endl;

        exit(1);
    }

    return Shader(location, shaderType);
}
