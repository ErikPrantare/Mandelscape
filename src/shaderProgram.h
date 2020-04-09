#ifndef MANDELLANDSCAPE_SHADERPROGRAM_H
#define MANDELLANDSCAPE_SHADERPROGRAM_H

#include <map>
#include <GL/glew.h>

#include "shader.h"
#include "math3d.h"

class ShaderProgram {
public:
    ShaderProgram(
            std::string const& vertexShader,
            std::string const& fragmentShader);

    ~ShaderProgram();

    void
    attatchShader(std::string const& path, GLenum const shaderType);

    void
    compile();

    void
    setUniform(const std::string& name, const Matrix4f& value);

    void
    setUniform(const std::string& name, float, float);

    void
    setTexture(
            unsigned char* const image,
            int width,
            int height,
            int nrChannels);

    void
    bindTexture();

private:
    GLuint m_location;
    GLuint m_textureLocation;
    bool m_textureLoaded = false;

    GLuint
    uniformLocation(const std::string& name) const;

    GLuint
    getLocation(std::string const& path, GLenum const shaderType);

    std::map<std::string, Shader> m_shaders;
};

#endif
