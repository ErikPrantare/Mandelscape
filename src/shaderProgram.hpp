#ifndef MANDELLANDSCAPE_SHADERPROGRAM_HPP
#define MANDELLANDSCAPE_SHADERPROGRAM_HPP

#include <map>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "glDestructors.hpp"

class ShaderProgram {
public:
    ShaderProgram() noexcept(false);

    auto
    attachShader(GLuint shader, GLenum shaderType) -> void;

    auto
    compile() noexcept(false) -> void;

    auto
    bindAttributeLocation(std::string const& name, int index) -> void;

    auto
    setUniformFloat(std::string const& name, float) -> void;

    auto
    setUniformInt(std::string const& name, int) -> void;

    auto
    setUniformVec2(std::string const& name, glm::vec2) -> void;

    auto
    setUniformVec3(std::string const& name, glm::vec3) -> void;

    auto
    setUniformMatrix4(std::string const& name, glm::mat4 const& value) -> void;

private:
    std::unique_ptr<GLuint, glDestructors::ShaderProgram> m_location;

    [[nodiscard]] auto
    uniformLocation(const std::string& name) const -> GLuint;

    [[nodiscard]] auto
    getLocation(std::string const& path, GLenum shaderType) const -> GLuint;
};

#endif
