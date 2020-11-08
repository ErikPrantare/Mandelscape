#ifndef MANDELSCAPE_SHADERCONTROLLER_HPP
#define MANDELSCAPE_SHADERCONTROLLER_HPP

#include <glm/glm.hpp>

#include "persistentActionMap.hpp"
#include "momentaryAction.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"

class ShaderController {
public:
    ShaderController(ShaderProgram* shaderProgram);

    auto
    handleMomentaryAction(MomentaryAction const&) -> void;

    auto
    updateState(PersistentActionMap const& active, double dt) -> void;

    auto
    update(ShaderProgram* shaderProgram) -> void;

private:
    VertexShader m_vertexShader =
            VertexShader::fromFile("shaders/shader.vert");

    int m_currentFragmentShader                     = 0;
    std::array<FragmentShader, 2> m_fragmentShaders = {
            FragmentShader::fromFile("shaders/shader.frag"),
            FragmentShader::fromFile("shaders/deepShader.frag")};

    bool m_fastMode     = false;
    bool m_switchShader = false;

    double m_colorFrequency  = 0.1;
    glm::dvec3 m_colorOffset = {0.0, 1.0, 2.0};
};

#endif
