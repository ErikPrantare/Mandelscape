#ifndef MANDELLANDSCAPE_SHADER_HPP
#define MANDELLANDSCAPE_SHADER_HPP

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
    auto
    operator=(Shader const&) -> Shader& = delete;
    Shader(Shader&&)                    = default;
    auto operator=(Shader &&) -> Shader& = default;
    ~Shader()                            = default;

    static auto
    fromFile(std::string const& filePath) -> Shader;

    static auto
    fromCode(std::string const& sourceCode) -> Shader;

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

    static auto
    createShader(std::string const& sourceCode) -> GLuint const*;
};

using VertexShader   = Shader<ShaderType::Vertex>;
using FragmentShader = Shader<ShaderType::Fragment>;

#endif
