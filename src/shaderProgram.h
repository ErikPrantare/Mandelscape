#ifndef MANDELLANDSCAPE_SHADERPROGRAM_H
#define MANDELLANDSCAPE_SHADERPROGRAM_H

#include <map>
#include <GL/glew.h>

#include "math3d.h"

class ShaderProgram {
public:
    ShaderProgram();

    ~ShaderProgram();

    void
    attatchShader(GLuint const shader, GLenum const shaderType) const;

    void
    compile() const;

    void
    setUniform(const std::string& name, const GLuint x) const;

    void
    setUniform(const std::string& name, const Matrix4f& value) const;

    void
    setUniform(const std::string& name, float const, float const) const;

private:
    GLuint m_location;

    GLuint
    uniformLocation(const std::string& name) const;

    GLuint
    getLocation(std::string const& path, GLenum const shaderType) const;
};

#endif
