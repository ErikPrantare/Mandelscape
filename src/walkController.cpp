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
    player->lookAtOffset.y =
            std::clamp(player->lookAtOffset.y, -pi / 2 + 0.001, pi / 2 - 0.001);
    m_dLookAtOffset = glm::dvec2{0.0, 0.0};

    auto rotator = glm::rotate(
            glm::dmat4(1.0),
            player->lookAtOffset.x,
            {0.0, 1.0, 0.0});
    player->position += dt * movementSpeed
                        * glm::dvec3(rotator * glm::dvec4(m_direction, 1.0));

    if(m_autoZoom) {
        player->scale = player->position.y;
    }
    else {
        player->scale *= std::exp(dt * m_scalingVelocity);
    }
}

auto
WalkController::handleEvent(Event const& event) -> void
{
    std::visit(
            util::Overload{
                    [this](KeyDown key) {
                        switch(key.code) {
                        case GLFW_KEY_W: {
                            m_direction.z += -1.0;
                        } break;
                        case GLFW_KEY_A: {
                            m_direction.x += -1.0;
                        } break;
                        case GLFW_KEY_S: {
                            m_direction.z += 1.0;
                        } break;
                        case GLFW_KEY_D: {
                            m_direction.x += 1.0;
                        } break;
                        case GLFW_KEY_J: {
                            m_scalingVelocity += -1.0;
                        } break;
                        case GLFW_KEY_K: {
                            m_scalingVelocity += 1.0;
                        } break;
                        case GLFW_KEY_O: {
                            m_autoZoom = !m_autoZoom;
                        } break;
                        }
                    },

                    [this](KeyUp key) {
                        switch(key.code) {
                        case GLFW_KEY_W: {
                            m_direction.z += 1.0;
                        } break;
                        case GLFW_KEY_A: {
                            m_direction.x += 1.0;
                        } break;
                        case GLFW_KEY_S: {
                            m_direction.z += -1.0;
                        } break;
                        case GLFW_KEY_D: {
                            m_direction.x += -1.0;
                        } break;
                        case GLFW_KEY_J: {
                            m_scalingVelocity += 1.0;
                        } break;
                        case GLFW_KEY_K: {
                            m_scalingVelocity += -1.0;
                        } break;
                        }
                    },

                    [this](MouseMove mouse) {
                        m_dLookAtOffset +=
                                util::pixelsToAngle({mouse.dx, mouse.dy});
                    },

                    // default
                    util::unaryNOP},
            event);
}
