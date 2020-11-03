#include "shader.hpp"

#include <string>
#include <iostream>
#include <array>

#include "util.hpp"

template<ShaderType type>
auto
Shader<type>::createShader(std::string const& sourceCode) -> GLuint const*
{
    auto const shaderType = static_cast<GLenum>(type);
    auto* const location  = new GLuint(glCreateShader(shaderType));

    if(*location == 0) {
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
        auto infoLog = std::array<GLchar, 1024>();
        glGetShaderInfoLog(
                *location,
                sizeof(infoLog),
                nullptr,
                infoLog.data());
        auto typeStr = shaderType == GL_FRAGMENT_SHADER ? std::string("FRAG")
                                                        : std::string("VERT");

        std::cerr << "Error compiling shader type " << typeStr << ": "
                  << "'" << infoLog.data() << "'" << std::endl;
        throw;
    }

    return location;
}

template<ShaderType type>
Shader<type>::Shader(std::string const& sourceCode) :
            m_location(createShader(sourceCode))
{}

template<ShaderType type>
auto
Shader<type>::fromFile(std::string const& filePath) -> Shader<type>
{
    return Shader<type>(util::readFile(filePath));
}

template<ShaderType type>
auto
Shader<type>::fromCode(std::string const& sourceCode) -> Shader<type>
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
