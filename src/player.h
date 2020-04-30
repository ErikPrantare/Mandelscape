#ifndef MANDELLANDSCAPE_PLAYER_H
#define MANDELLANDSCAPE_PLAYER_H

#include "event.h"
#include "utils.h"
#include "mandelTypeTraits.h"

class Player {
public:
    void
    handleEvent(Event const& event, Config* const config)
    {
        std::visit(
                util::overload{
                        [this, config](KeyDown key) {
                            switch(key.key) {
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
                            }
                        },

                        [this, config](KeyUp key) {
                            switch(key.key) {
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

                        // default
                        [](auto x) -> Require<!SpecialisationOf<
                                           decltype(x),
                                           std::variant>::value> {
                        }},
                event);
    }

    void
    update(double dt)
    {
        m_position += float(dt) * float(m_scale) * m_velocity;
        m_scale *= std::exp(m_scaleVelocity * dt);
    }

    glm::vec3 m_velocity   = glm::vec3(0, 0, 0);
    glm::vec3 m_position   = glm::vec3(0, 0, 0);
    double m_scale         = 1.0;
    double m_scaleVelocity = 0.0;

    static constexpr float m_movementSpeed = 1.0;
};

#endif
