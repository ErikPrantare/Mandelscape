#include "colorController.hpp"

#include <cmath>

auto
ColorController::update(PersistentActionMap const& active, double dt) -> void
{
    int change = (active(PersistentAction::IncreaseParam) ? 1 : 0)
                 - (active(PersistentAction::DecreaseParam) ? 1 : 0);

    if(active(PersistentAction::ChangeFrequency)) {
        m_colorFrequency *= std::exp(change * dt);
    }
    if(active(PersistentAction::ChangeRedOffset)) {
        m_colorOffset.x += 0.3 * change / m_colorFrequency * dt;
    }
    if(active(PersistentAction::ChangeGreenOffset)) {
        m_colorOffset.y += 0.3 * change / m_colorFrequency * dt;
    }
    if(active(PersistentAction::ChangeBlueOffset)) {
        m_colorOffset.z += 0.3 * change / m_colorFrequency * dt;
    }
}

auto
ColorController::updateShaderVariables(ShaderProgram* program) const -> void
{
    program->setUniformFloat("colorFrequency", (float)m_colorFrequency);
    program->setUniformVec3("colorOffset", m_colorOffset);
}
