#ifndef MANDELSCAPE_FRAMEBUFFER_HPP
#define MANDELSCAPE_FRAMEBUFFER_HPP

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "glDestructors.hpp"

class Framebuffer {
public:
    Framebuffer(glm::ivec2 size) noexcept(false);

    [[nodiscard]] auto
    size() const noexcept -> glm::ivec2;

    [[nodiscard]] auto
    readPixels() -> std::vector<unsigned char>;

    auto
    bind() noexcept -> void;

    static auto
    unbind() noexcept -> void;

private:
    std::unique_ptr<GLuint, glDestructors::Fbo> m_fbo{new GLuint};
    Texture m_texture;
    Texture m_depth;
    glm::ivec2 m_size;
};

#endif
