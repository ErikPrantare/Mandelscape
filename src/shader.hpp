#ifndef MANDELLANDSCAPE_SHADER_HPP
#define MANDELLANDSCAPE_SHADER_HPP

#include <string>
#include <memory>
#include <iostream>
#include <type_traits>

#include <glad/glad.h>

#include "shaderProgram.hpp"
#include "glDestructors.hpp"
#include "util.hpp"

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

    template<class Arg, class... Args>
    static auto
    fromFiles(Arg filePath, Args... filePaths) -> Shader
    {
        return Shader<type>(util::concatFiles(filePath, filePaths...));
    }

    static auto
    fromCode(std::string const& sourceCode) -> Shader;

    auto
    attachTo(ShaderProgram& program) const -> void;

private:
    Shader(std::string const& sourceCode);

    std::unique_ptr<GLuint const, glDestructors::Shader> m_location;

    static auto
    createShader(std::string const& sourceCode) -> GLuint const*;
};

using VertexShader   = Shader<ShaderType::Vertex>;
using FragmentShader = Shader<ShaderType::Fragment>;

#endif
