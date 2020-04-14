#ifndef MANDELLANDSCAPE_SHADERPROGRAM_H
#define MANDELLANDSCAPE_SHADERPROGRAM_H

#include <map>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "math3d.h"

class ShaderProgram {
public:
    ShaderProgram();

    ShaderProgram(ShaderProgram const&) = delete;
    ShaderProgram&
    operator=(ShaderProgram const&) = delete;

    void
    attachShader(GLuint const shader, GLenum const shaderType) const;

    void
    compile() const;

    void
    setUniform(const std::string& name, const GLuint x) const;

    void
    setUniform(const std::string& name, const glm::mat4& value) const;

    void
    setUniform(const std::string& name, float const, float const) const;

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
