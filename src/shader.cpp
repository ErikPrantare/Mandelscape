#include <string>
#include <iostream>

#include "shader.h"
#include "utils.h"

Shader
Shader::fromFile(const std::string& filename, GLenum shaderType)
{
    return fromCode(readFile(filename), shaderType);
}

Shader
Shader::fromCode(const std::string& code, GLenum shaderType)
{
    Shader shader;

    shader.m_location = glCreateShader(shaderType);
    shader.m_type     = shaderType;

    if(shader.m_location == 0) {
        std::cerr << "Error creating shader type " << shaderType << std::endl;
        exit(1);
    }

    const GLchar* p[1];
    p[0] = code.c_str();
    GLint lengths[1];
    lengths[0] = code.size();
    glShaderSource(shader.m_location, 1, p, lengths);
    glCompileShader(shader.m_location);
    GLint success;
    glGetShaderiv(shader.m_location, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(
                shader.m_location,
                sizeof(infoLog),
                nullptr,
                infoLog);
        std::cerr << "Error compiling shader type " << shaderType << ": "
                  << "'" << infoLog << "'" << std::endl;

        exit(1);
    }

    return shader;
}
