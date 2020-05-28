#ifndef MANDELLANDSCAPE_SHADER_H
#define MANDELLANDSCAPE_SHADER_H

#include <string>
#include <memory>
#include <iostream>
#include <type_traits>

#include <glad/glad.h>

#include "shaderProgram.hpp"

enum class ShaderType {
    Vertex   = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER
};

template<ShaderType type>
class Shader {
public:
    Shader()              = delete;
    Shader(Shader const&) = delete;
    Shader&
    operator=(Shader const&) = delete;
    Shader(Shader&&)         = default;
    Shader&
    operator=(Shader&&) = default;
    ~Shader()           = default;

    static Shader
    fromFile(std::string const& filePath);

    static Shader
    fromCode(std::string const& sourceCode);

    void
    attachTo(ShaderProgram& program) const;

private:
    struct LocationDeleter {
        void
        operator()(GLuint const* location) noexcept
        {
            glDeleteShader(*location);
            delete location;
        }
    };

    Shader(std::string const& sourceCode);

    std::unique_ptr<GLuint const, LocationDeleter> m_location;

    static GLuint const*
    createShader(std::string const& sourceCode);
};

using VertexShader   = Shader<ShaderType::Vertex>;
using FragmentShader = Shader<ShaderType::Fragment>;

#endif
