#ifndef MANDELLANDSCAPE_SHADER_HPP
#define MANDELLANDSCAPE_SHADER_HPP

#include <string>
#include <memory>
#include <iostream>
#include <type_traits>

#include <glad/glad.h>

#include "shaderProgram.hpp"
#include "glDestructors.hpp"

enum class ShaderType {
    Vertex   = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER
};

template<ShaderType type>
class Shader {
public:
    Shader() = delete;

    static auto
    fromFile(std::string const& filePath) -> Shader;

    static auto
    fromCode(std::string const& sourceCode) -> Shader;

    void
    attachTo(ShaderProgram& program) const;

private:
    Shader(std::string const& sourceCode);

    std::unique_ptr<GLuint const, glDestructors::Shader> m_location;

    static auto
    createShader(std::string const& sourceCode) -> GLuint const*;
};

using VertexShader   = Shader<ShaderType::Vertex>;
using FragmentShader = Shader<ShaderType::Fragment>;

#endif
