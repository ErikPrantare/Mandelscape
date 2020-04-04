#ifndef MANDELLANDSCAPE_SHADERPROGRAM_H
#define MANDELLANDSCAPE_SHADERPROGRAM_H

#include <GL/glew.h>

#include "shader.h"

class ShaderProgram {
public:
    ShaderProgram();

    void
    useShader(const Shader& shader);

    void
    compile();

private:
    GLuint m_location;
};

#endif
