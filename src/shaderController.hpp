#ifndef MANDELSCAPE_SHADERCONTROLLER_HPP
#define MANDELSCAPE_SHADERCONTROLLER_HPP

#include <glm/glm.hpp>

#include "persistentActionMap.hpp"
#include "momentaryAction.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"

/* Control shader parameters, e.g. coloring */
class ShaderController {
public:
    ShaderController(ShaderProgram* shaderProgram);

    auto
    handleMomentaryAction(MomentaryAction const&) -> void;

    auto
    updateState(PersistentActionMap const& active, double dt) -> void;

    auto
    update(ShaderProgram* shaderProgram) -> void;

    [[nodiscard]] auto
    yScale() const -> double;

private:
    VertexShader m_vertexShader =
            VertexShader::fromFile("shaders/shader.vert");

    int m_currentFragmentShader                     = 0;
    std::array<FragmentShader, 2> m_fragmentShaders = {
            FragmentShader::fromFiles(
                    "shaders/head.frag",
                    "shaders/shallowLib.frag",
                    "shaders/value.frag",
                    "shaders/color.frag",
                    "shaders/shader.frag"),
            FragmentShader::fromFiles(
                    "shaders/head.frag",
                    "shaders/deepLib.frag",
                    "shaders/value.frag",
                    "shaders/color.frag",
                    "shaders/shader.frag")};

    bool m_fastMode     = false;
    bool m_switchShader = false;

    double m_colorFrequency  = 0.1;
    glm::dvec3 m_colorOffset = {0.0, 1.0, 2.0};
    double m_yScale          = 1.0;
};

#endif
