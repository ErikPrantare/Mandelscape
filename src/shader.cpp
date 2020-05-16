#include "shader.h"

#include <string>
#include <iostream>

#include "utils.h"

GLuint const*
Shader::createShader(std::string const& sourceCode, GLenum const shaderType)
{
    auto* const location = new GLuint(glCreateShader(shaderType));

    if(location == 0) {
        std::cerr << "Error creating shader type " << shaderType << std::endl;
        throw;
    }

    GLchar const* charSource = sourceCode.c_str();
    GLint const shaderLength = sourceCode.length();
    glShaderSource(*location, 1, &charSource, &shaderLength);
    glCompileShader(*location);

    GLint success;
    glGetShaderiv(*location, GL_COMPILE_STATUS, &success);
    if(success == 0) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(*location, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Error compiling shader type "
                  << (shaderType == GL_FRAGMENT_SHADER ? "FRAG" : "VERT")
                  << ": "
                  << "'" << infoLog << "'" << std::endl;

        exit(1);
    }

    return location;
}

Shader::Shader(std::string const& sourceCode, GLenum const shaderType) :
            type(shaderType),
            m_location(createShader(sourceCode, shaderType))
{}

Shader
Shader::fromFile(std::string const& filePath, GLenum const shaderType)
{
    return Shader(util::readFile(filePath), shaderType);
}

Shader
Shader::fromCode(std::string const& sourceCode, GLenum const shaderType)
{
    return Shader(sourceCode, shaderType);
}

void
Shader::attachTo(ShaderProgram& program) const
{
    program.attachShader(*m_location, type);
}
