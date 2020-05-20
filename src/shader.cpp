#include "shader.h"

#include <string>
#include <iostream>

#include "utils.h"

template<ShaderType type>
GLuint const*
Shader<type>::createShader(std::string const& sourceCode)
{
    auto shaderType      = static_cast<GLenum>(type);
    auto* const location = new GLuint(glCreateShader(shaderType));

    if(location == 0) {
        std::cerr << "Error creating shader type " << shaderType << std::endl;
        throw;
    }

    GLchar const* charSource = sourceCode.c_str();
    GLint const shaderLength = sourceCode.length();
    glShaderSource(*location, 1, &charSource, &shaderLength);
    glCompileShader(*location);

    GLint success = 0;
    glGetShaderiv(*location, GL_COMPILE_STATUS, &success);
    if(success == 0) {
        auto infoLog = std::basic_string<GLchar>(1024, ' ');
        glGetShaderInfoLog(*location, sizeof(infoLog), nullptr, infoLog.data());
        auto typeStr = shaderType == GL_FRAGMENT_SHADER ? std::string("FRAG")
                                                        : std::string("VERT");

        std::cerr << "Error compiling shader type " << typeStr << ": "
                  << "'" << infoLog << "'" << std::endl;
        throw;
    }

    return location;
}

template<ShaderType type>
Shader<type>::Shader(std::string const& sourceCode) :
            m_location(createShader(sourceCode))
{}

template<ShaderType type>
Shader<type>
Shader<type>::fromFile(std::string const& filePath)
{
    return Shader<type>(util::readFile(filePath));
}

template<ShaderType type>
Shader<type>
Shader<type>::fromCode(std::string const& sourceCode)
{
    return Shader<type>(sourceCode);
}

template<ShaderType type>
void
Shader<type>::attachTo(ShaderProgram& program) const
{
    program.attachShader(*m_location, static_cast<GLenum>(type));
}

template class Shader<ShaderType::Vertex>;
template class Shader<ShaderType::Fragment>;
