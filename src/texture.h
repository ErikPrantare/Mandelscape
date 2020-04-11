#ifndef MANDELLANDSCAPE_TEXTURE_H
#define MANDELLANDSCAPE_TEXTURE_H

#include <string>
#include <memory>

#include <GL/glew.h>

class Texture {
public:
    Texture() = delete;

    Texture(std::string const& path);

    void
    makeActiveOn(GLenum const textureUnit) const;

private:
    struct TextureDeleter {
        void
        operator()(GLuint* location)
        {
            glDeleteTextures(1, location);
            delete location;
        }
    };

    std::unique_ptr<GLuint, TextureDeleter> m_location;
};

#endif    // MANDELLANDSCAPE_TEXTURE_H
