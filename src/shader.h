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

    GLuint
    location()
    {
        return *m_location;
    }

private:
    std::shared_ptr<GLuint const> m_location;

    static GLuint const*
    createShader(std::string const& source, GLenum shaderType);
};

#endif
