#ifndef MANDELLANDSCAPE_PLAYER_H
#define MANDELLANDSCAPE_PLAYER_H

#include "event.h"
#include "utils.h"
#include "mandelTypeTraits.h"

class Player {
public:
    void
    handleEvent(Event const& event)
    {
        std::visit(
                util::overload{
                        [this](KeyDown key) {
                            switch(key.code) {
                            case GLFW_KEY_W: {
                                m_velocity.z += -m_movementSpeed;
                            } break;
                            case GLFW_KEY_A: {
                                m_velocity.x += -m_movementSpeed;
                            } break;
                            case GLFW_KEY_S: {
                                m_velocity.z += m_movementSpeed;
                            } break;
                            case GLFW_KEY_D: {
                                m_velocity.x += m_movementSpeed;
                            } break;
                            case GLFW_KEY_J: {
                                m_scaleVelocity += -1.f;
                            } break;
                            case GLFW_KEY_K: {
                                m_scaleVelocity += 1.f;
                            } break;
                            case GLFW_KEY_O: {
                                m_autoZoom ^= true;
                            } break;
                            }
                        },

                        [this](KeyUp key) {
                            switch(key.code) {
                            case GLFW_KEY_W: {
                                m_velocity.z += m_movementSpeed;
                            } break;
                            case GLFW_KEY_A: {
                                m_velocity.x += m_movementSpeed;
                            } break;
                            case GLFW_KEY_S: {
                                m_velocity.z += -m_movementSpeed;
                            } break;
                            case GLFW_KEY_D: {
                                m_velocity.x += -m_movementSpeed;
                            } break;
                            case GLFW_KEY_J: {
                                m_scaleVelocity += 1.f;
                            } break;
                            case GLFW_KEY_K: {
                                m_scaleVelocity += -1.f;
                            } break;
                            }
                        },

                        [this](MouseMove mouse) {
                            m_lookAtOffset.x += mouse.dx / 100.f;
                            m_lookAtOffset.y += mouse.dy / 100.f;
                            m_lookAtOffset.y = std::clamp(
                                    m_lookAtOffset.y,
                                    float(-glm::pi<double>() / 2 + 0.001),
                                    float(glm::pi<double>() / 2 - 0.001));
                        },
                        // default
                        [](auto x) {
                        }},
                event);
    }

    void
    update(double dt)
    {
        m_position += float(dt) * float(m_scale) * m_velocity;
        if(m_autoZoom) {
            m_scale = m_position.y;
        }
        else {
            m_scale *= std::exp(m_scaleVelocity * dt);
        }
    }

    glm::vec3 m_velocity   = glm::vec3(0, 0, 0);
    glm::vec3 m_position   = glm::vec3(0, 0, 0);
    double m_scale         = 1.0;
    double m_scaleVelocity = 0.0;
    bool m_autoZoom        = false;

    glm::vec2 m_lookAtOffset = {0.0, 0.0};

    static constexpr float m_movementSpeed = 1.0;
};

#endif
