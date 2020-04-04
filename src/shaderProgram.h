#ifndef MANDELLANDSCAPE_SHADERPROGRAM_H
#define MANDELLANDSCAPE_SHADERPROGRAM_H

#include <GL/glew.h>

#include "shader.h"
#include "math3d.h"

class ShaderProgram {
public:
    ShaderProgram();

    void
    useShader(const Shader& shader);

    void
    compile();

    void
    setUniform(const std::string& name, const Matrix4f& value);

private:
    GLuint m_location;

    GLuint
    locationOf(const std::string& name) const;
};

#endif
