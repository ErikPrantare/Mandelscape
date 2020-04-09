#ifndef MANDELLANDSCAPE_SHADER_H
#define MANDELLANDSCAPE_SHADER_H

#include <string>
#include <memory>
#include <filesystem>
#include <iostream>
#include <type_traits>

#include <GL/glew.h>

#include "utils.h"

class Shader {
public:
    Shader() = delete;

    Shader(std::string const& filePath, GLenum const shaderType);

    GLenum const type;
    std::string const path;

    GLuint
    location()
    {
        return *m_location;
    }

private:
    struct ShaderLocation {
        GLuint const value;

        operator GLuint() const
        {
            return value;
        }

        GLuint operator*()
        {
            return value;
        }

        template<
                typename Null,
                typename = std::enable_if_t<std::is_null_pointer_v<Null>>>
        constexpr bool
        operator==(Null rhs) const
        {
            return value > 0;
        }

        template<
                typename Null,
                typename = std::enable_if_t<std::is_null_pointer_v<Null>>>
        constexpr bool
        operator!=(Null rhs) const
        {
            return !(value == rhs);
        }
    };

    struct ShaderDeleter {
        using pointer = ShaderLocation;
        void
        operator()(pointer location) const
        {
            glDeleteShader(location);
        }
    };

    std::unique_ptr<ShaderLocation, ShaderDeleter> m_location;

    static GLuint
    createShader(std::string const& source, GLenum shaderType);
};

#endif
