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
    ShaderProgram&
    operator=(ShaderProgram const&) = delete;
    ShaderProgram(ShaderProgram&&)  = default;
    ShaderProgram&
    operator=(ShaderProgram&&) = default;
    ~ShaderProgram()           = default;

    void
    attachShader(GLuint shader, GLenum shaderType);

    void
    compile();

    void
    setUniformFloat(std::string const& name, float);

    void
    setUniformInt(std::string const& name, int);

    void
    setUniformVec2(std::string const& name, float, float);

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

    GLuint
    uniformLocation(const std::string& name) const;

    GLuint
    getLocation(std::string const& path, GLenum shaderType) const;
};

#endif
