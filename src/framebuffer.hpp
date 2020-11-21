#ifndef MANDELSCAPE_FRAMEBUFFER_HPP
#define MANDELSCAPE_FRAMEBUFFER_HPP

#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "glDestructors.hpp"

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
    std::unique_ptr<GLuint, glDestructors::Fbo> m_fbo{new GLuint};
    std::unique_ptr<GLuint, glDestructors::Texture> m_texture{new GLuint};
    std::unique_ptr<GLuint, glDestructors::Texture> m_depth{new GLuint};
    glm::ivec2 m_size;
};

#endif
