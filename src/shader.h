#ifndef MANDELLANDSCAPE_SHADER_H
#define MANDELLANDSCAPE_SHADER_H

#include <string>
#include <memory>
#include <filesystem>
#include <iostream>

#include <GL/glew.h>

#include "utils.h"

struct ShaderLocation {
    GLuint const location;

    explicit ShaderLocation(GLuint const shaderLocation) :
                location(shaderLocation)
    {}

    ShaderLocation(ShaderLocation const&) = delete;
    ShaderLocation(ShaderLocation&&)      = delete;

    operator GLuint() const
    {
        return location;
    }

    ~ShaderLocation()
    {
        glDeleteShader(location);
    }
};

class Shader {
public:
    Shader() = delete;

    Shader(std::string const& filePath, GLenum const shaderType);

    std::shared_ptr<ShaderLocation const> const location;
    GLenum const type;

private:
    static std::shared_ptr<ShaderLocation const>
    createShader(std::string const& source, GLenum shaderType);
};

#endif
