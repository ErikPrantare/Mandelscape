#ifndef MANDELSCAPE_GL_DESTRUCTORS_HPP
#define MANDELSCAPE_GL_DESTRUCTORS_HPP

#include <glad/glad.h>

namespace glDestructors {

// CPP20 decltype(lambda)
class Fbo {
public:
    auto
    operator()(GLuint* location) -> void
    {
        glDeleteFramebuffers(1, location);
        delete location;
    }
};

class Texture {
public:
    void
    operator()(GLuint* location) noexcept
    {
        glDeleteTextures(1, location);
        delete location;
    }
};

}    // namespace glDestructors

#endif
