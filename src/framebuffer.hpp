#ifndef MANDELSCAPE_FRAMEBUFFER_HPP
#define MANDELSCAPE_FRAMEBUFFER_HPP

#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Framebuffer {
public:
    Framebuffer(glm::ivec2 size) noexcept(false);

    [[nodiscard]] auto
    size() const noexcept -> glm::ivec2;

    auto
    bind() noexcept -> void;

    auto
    unbind() noexcept -> void;

private:
    // CPP20 decltype(lambda)
    class FboDeleter {
    public:
        auto
        operator()(GLuint* location) -> void
        {
            glDeleteFramebuffers(1, location);
            delete location;
        }
    };

    std::unique_ptr<GLuint, FboDeleter> m_fbo;
    glm::ivec2 m_size;
    GLuint texture = 0;
    GLuint m_depth = 0;
};

#endif
