#include "walkController.hpp"

#include <algorithm>

#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "utils.hpp"

auto
WalkController::update(Player* const player, double const dt) -> void
{
    auto constexpr pi = glm::pi<double>();

    player->lookAtOffset += m_dLookAtOffset;
    player->lookAtOffset.y = std::clamp(
            player->lookAtOffset.y,
            -pi / 2 + 0.001,
            pi / 2 - 0.001);
    m_dLookAtOffset = glm::dvec2{0.0, 0.0};

    auto rotator = glm::rotate(
            glm::dmat4(1.0),
            player->lookAtOffset.x,
            {0.0, 1.0, 0.0});
    player->position += dt * player->scale * movementSpeed
                        * glm::dvec3(rotator * glm::dvec4(m_direction, 1.0));

    if(!m_autoZoom) {
        player->scale *= std::exp(dt * m_scalingVelocity);
    }
    else if(player->position.y != 0.0) {
        player->scale = player->position.y;
    }
}

auto
WalkController::handleMomentaryAction(MomentaryAction const& action) -> void
{
    std::visit(
            util::Overload{
                    [this](TriggerAction action) {
                        if(action == TriggerAction::ToggleAutoZoom) {
                            m_autoZoom = !m_autoZoom;
                        }
                    },

                    [this](MouseDelta mouse) {
                        m_dLookAtOffset +=
                                util::pixelsToAngle({mouse.dx, mouse.dy});
                    },

                    // default
                    util::unaryNOP},
            action);
}
auto
WalkController::updateState(PersistentActionMap const& active) -> void
{
    m_direction = {0, 0, 0};
    if(active(PersistentAction::MoveForwards)) {
        m_direction.z -= 1.0;
    }
    if(active(PersistentAction::MoveBackwards)) {
        m_direction.z += 1.0;
    }
    if(active(PersistentAction::MoveLeft)) {
        m_direction.x -= 1.0;
    }
    if(active(PersistentAction::MoveRight)) {
        m_direction.x += 1.0;
    }

    m_scalingVelocity = 0.0;
    if(active(PersistentAction::ZoomIn)) {
        m_scalingVelocity -= 1.0;
    }
    if(active(PersistentAction::ZoomOut)) {
        m_scalingVelocity += 1.0;
    }
}
