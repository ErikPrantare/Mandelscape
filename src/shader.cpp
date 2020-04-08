#include "shader.h"

#include <string>
#include <iostream>

#include "utils.h"

Shader::Shader(std::string const& filePath, GLenum const shaderType) :
            location(createShader(readFile(filePath), shaderType)),
            type(shaderType)
{}

std::shared_ptr<ShaderLocation const>
Shader::createShader(std::string const& source, GLenum shaderType)
{
    GLuint const location = glCreateShader(shaderType);

    if(location == 0) {
        std::cerr << "Error creating shader type " << shaderType << std::endl;
        exit(1);
    }

    GLchar const* charSource = source.c_str();
    GLint const shaderLength = source.length();
    glShaderSource(location, 1, &charSource, &shaderLength);
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

    return std::make_shared<ShaderLocation const>(location);
}
