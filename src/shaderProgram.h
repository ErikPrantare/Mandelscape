#ifndef MANDELLANDSCAPE_SHADERPROGRAM_H
#define MANDELLANDSCAPE_SHADERPROGRAM_H

#include <map>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

class ShaderProgram {
public:
    ShaderProgram();

    ShaderProgram(ShaderProgram const&) = delete;
    ShaderProgram&
    operator=(ShaderProgram const&) = delete;

    void
    attachShader(GLuint const shader, GLenum const shaderType);

    void
    compile();

    void
    setUniformFloat(std::string const& name, float const);

    void
    setUniformInt(std::string const& name, int const);

    void
    setUniformVec2(std::string const& name, float const, float const);

    void
    setUniformMatrix4(std::string const& name, glm::mat4 const& value);

private:
    struct ShaderDeleter {
        void
        operator()(GLuint* location) noexcept
        {
            glDeleteProgram(*location);
            delete location;
        }
    };

    std::unique_ptr<GLuint, ShaderDeleter> m_location;

    GLuint
    uniformLocation(const std::string& name) const;

    GLuint
    getLocation(std::string const& path, GLenum const shaderType) const;
};

#endif
