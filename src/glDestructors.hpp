#ifndef MANDELSCAPE_GL_DESTRUCTORS_HPP
#define MANDELSCAPE_GL_DESTRUCTORS_HPP

#include <glad/glad.h>

namespace glDestructors {

// CPP20 decltype(lambda)
class Fbo {
public:
    auto
    operator()(GLuint* location) noexcept -> void
    {
        glDeleteFramebuffers(1, location);
        delete location;
    }
};

class Texture {
public:
    auto
    operator()(GLuint* location) noexcept -> void
    {
        glDeleteTextures(1, location);
        delete location;
    }
};

struct Shader {
    auto
    operator()(GLuint const* location) noexcept -> void
    {
        glDeleteShader(*location);
        delete location;
    }
};

struct ShaderProgram {
    auto
    operator()(GLuint const* location) noexcept -> void
    {
        glDeleteProgram(*location);
        delete location;
    }
};
}    // namespace glDestructors

#endif
