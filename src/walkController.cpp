#include "walkController.hpp"

#include <GLFW/glfw3.h>

#include "utils.hpp"

glm::dvec3
WalkController::velocity() const
{
    return m_movementSpeed * m_direction;
}

void
WalkController::handleEvent(Event const& event)
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
                        }
                    },
                    // default
                    util::unaryNOP},
            event);
}
