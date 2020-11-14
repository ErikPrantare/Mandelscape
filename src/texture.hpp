#ifndef MANDELLANDSCAPE_TEXTURE_HPP
#define MANDELLANDSCAPE_TEXTURE_HPP

#include <string>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

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

    Texture() = delete;

    Texture(Texture const&) = delete;
    auto
    operator=(Texture const&) -> Texture& = delete;

    Texture(Texture&&) = default;
    auto operator=(Texture &&) -> Texture& = default;

    ~Texture() = default;

    auto
    activate() -> void;

private:
    struct TextureDeleter {
        void
        operator()(GLuint* location) noexcept
        {
            glDeleteTextures(1, location);
            delete location;
        }
    };

    std::unique_ptr<GLuint, TextureDeleter> m_location;

    GLenum m_textureUnit;
};

#endif    // MANDELLANDSCAPE_TEXTURE_HPP
