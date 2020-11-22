#ifndef MANDELLANDSCAPE_SHADERPROGRAM_HPP
#define MANDELLANDSCAPE_SHADERPROGRAM_HPP

#include <map>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class ShaderProgram {
public:
    ShaderProgram();

    ShaderProgram(ShaderProgram const&) = delete;
    auto
    operator=(ShaderProgram const&) -> ShaderProgram& = delete;
    ShaderProgram(ShaderProgram&&)                    = default;
    auto operator=(ShaderProgram &&) -> ShaderProgram& = default;
    ~ShaderProgram()                                   = default;

    void
    attachShader(GLuint shader, GLenum shaderType);

    void
    compile();

    auto
    bindAttributeLocation(std::string const& name, int index) -> void;

    void
    setUniformFloat(std::string const& name, float);

    void
    setUniformInt(std::string const& name, int);

    void
    setUniformVec2(std::string const& name, glm::vec2);

    void
    setUniformVec3(std::string const& name, glm::vec3);

    void
    setUniformMatrix4(std::string const& name, glm::mat4 const& value);

private:
    struct ShaderDeleter {
        void
        operator()(const GLuint* location) noexcept
        {
            glDeleteProgram(*location);
            delete location;
        }
    };

    std::unique_ptr<GLuint, ShaderDeleter> m_location;

    [[nodiscard]] auto
    uniformLocation(const std::string& name) const -> GLuint;

    [[nodiscard]] auto
    getLocation(std::string const& path, GLenum shaderType) const -> GLuint;
};

#endif
