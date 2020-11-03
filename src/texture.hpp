#ifndef MANDELLANDSCAPE_TEXTURE_HPP
#define MANDELLANDSCAPE_TEXTURE_HPP

#include <string>
#include <memory>

#include <glad/glad.h>

class Texture {
public:
    Texture(std::string const& path, GLenum textureUnit = GL_TEXTURE0);

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
