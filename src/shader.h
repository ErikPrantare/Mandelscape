#ifndef MANDELLANDSCAPE_SHADER_H
#define MANDELLANDSCAPE_SHADER_H

#include <string>
#include <memory>
#include <iostream>
#include <type_traits>

#include <GL/glew.h>

#include "utils.h"
#include "shaderProgram.h"

class Shader {
public:
    Shader()              = delete;
    Shader(Shader const&) = delete;
    Shader&
    operator=(Shader const&) = delete;

    static Shader
    fromFile(std::string const& filePath, GLenum const shaderType);

    static Shader
    fromCode(std::string const& sourceCode, GLenum const shaderType);

    void
    attachTo(ShaderProgram const& program) const;

    GLenum const type;

private:
    struct LocationDeleter {
        void
        operator()(GLuint const* location) noexcept
        {
            glDeleteShader(*location);
            delete location;
        }
    };

    Shader(std::string const& sourceCode, GLenum const shaderType);

    std::unique_ptr<GLuint const, LocationDeleter> m_location;

    static GLuint const*
    createShader(std::string const& sourceCode, GLenum const shaderType);
};

#endif
