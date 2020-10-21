#ifndef MANDELLANDSCAPE_TEXTURE_HPP
#define MANDELLANDSCAPE_TEXTURE_HPP

#include <string>
#include <memory>

#include <glad/glad.h>

class Texture {
public:
    Texture(std::string const& path);

    Texture()               = delete;
    Texture(Texture const&) = delete;
    Texture&
    operator=(Texture const&) = delete;
    Texture(Texture&&)        = default;
    Texture&
    operator=(Texture&&) = default;
    ~Texture()           = default;

    void
    makeActiveOn(GLenum textureUnit) const;

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
};

#endif    // MANDELLANDSCAPE_TEXTURE_HPP
