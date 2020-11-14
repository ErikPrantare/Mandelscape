#ifndef MANDELSCAPE_FRAMEBUFFER_HPP
#define MANDELSCAPE_FRAMEBUFFER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

class Framebuffer {
public:
    Framebuffer(glm::ivec2 size) noexcept(false);
    ~Framebuffer();

    [[nodiscard]] auto
    size() const noexcept -> glm::ivec2;

    auto
    bind() noexcept -> void;

    auto
    unbind() noexcept -> void;

private:
    GLuint m_fbo;
    glm::ivec2 m_size;
    GLuint texture;
};

#endif
