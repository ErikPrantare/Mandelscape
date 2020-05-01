#ifndef MANDELLANDSCAPE_PLAYER_H
#define MANDELLANDSCAPE_PLAYER_H

#include <glm/gtx/rotate_vector.hpp>

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
    update(glm::vec2 terrainOffset, double dt)
    {
        // HACK: m_position -= instead of +=.
        // -m_lookAtOffset.x. Probably some spooky stuff with
        // the coordinate system again. Look into.
        m_position -= float(dt) * float(m_scale)
                      * glm::rotateY(m_velocity, -m_lookAtOffset.x);
        if(m_autoZoom) {
            m_scale = m_position.y;
        }
        else {
            m_scale *= std::exp(m_scaleVelocity * dt);
        }

        auto dPos       = terrainOffset - m_terrainOffset;
        m_terrainOffset = terrainOffset;
        m_position.x -= dPos.x;
        m_position.z -= dPos.y;
    }

    glm::vec3
    absolutePosition()
    {
        return m_position
               + glm::vec3(m_terrainOffset.x, 0.f, m_terrainOffset.y);
    }

    glm::vec3 m_velocity      = glm::vec3(0, 0, 0);
    glm::vec3 m_position      = glm::vec3(0, 0, 0);
    glm::vec2 m_terrainOffset = glm::vec2(0, 0);
    double m_scale            = 1.0;
    double m_scaleVelocity    = 0.0;
    bool m_autoZoom           = false;

    glm::vec2 m_lookAtOffset = glm::vec2(0.0, 0.0);

    static constexpr float m_movementSpeed = 1.0;
};

#endif
