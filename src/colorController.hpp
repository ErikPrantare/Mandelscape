#ifndef MANDELLANDSCAPE_COLORCONTROLLER_HPP
#define MANDELLANDSCAPE_COLORCONTROLLER_HPP

#include <glm/glm.hpp>

#include "persistentActionMap.hpp"
#include "shaderProgram.hpp"

class ColorController {
public:
    auto
    update(PersistentActionMap const& active, double dt) -> void;

    auto
    updateShaderVariables(ShaderProgram* program) const -> void;

private:
    double m_colorFrequency  = 0.1;
    glm::dvec3 m_colorOffset = {0.0, 1.0, 2.0};
};

#endif
