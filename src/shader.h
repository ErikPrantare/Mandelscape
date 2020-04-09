#ifndef MANDELLANDSCAPE_SHADER_H
#define MANDELLANDSCAPE_SHADER_H

#include <string>
#include <memory>
#include <iostream>
#include <type_traits>

#include <GL/glew.h>

#include "utils.h"

class Shader {
public:
    Shader() = delete;

    Shader(std::string const& filePath, GLenum const shaderType);

    GLenum const type;

    GLuint
    location()
    {
        return *m_location;
    }

private:
    struct LocationDeleter {
        void
        operator()(GLuint const* location) noexcept
        {
            glDeleteShader(*location);
            delete location;
        }
    };

    std::unique_ptr<GLuint const, LocationDeleter> m_location;

    static GLuint const*
    createShader(std::string const& source, GLenum shaderType);
};

#endif
