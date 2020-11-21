#ifndef MANDELLANDSCAPE_TEXTURE_HPP
#define MANDELLANDSCAPE_TEXTURE_HPP

#include <string>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "glDestructors.hpp"

struct TextureArgs {
    glm::ivec2 size{0, 0};
    GLenum unit           = GL_TEXTURE0;
    GLint format          = GL_RGBA;
    std::string imagePath = "";
    bool generateMipmap   = false;
};

class Texture {
public:
    Texture(TextureArgs const& args) noexcept(false);

    auto
    activate() -> void;

private:
    std::unique_ptr<GLuint, glDestructors::Texture> m_location;

    GLenum m_textureUnit;
};

#endif    // MANDELLANDSCAPE_TEXTURE_HPP
