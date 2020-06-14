#include "player.hpp"

#include <algorithm>
#include <functional>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <GLFW/glfw3.h>

#include "event.hpp"
#include "utils.hpp"
#include "mandelTypeTraits.hpp"

void
Player::handleEvent(Event const event)
{
    // CPP20
    // https://en.cppreference.com/w/cpp/utility/functional/bind_front
    std::visit(
            util::Overload{
                    [this](KeyDown keyEvent) { keyDown(keyEvent); },
                    [this](KeyUp keyEvent) { keyUp(keyEvent); },
                    // default
                    util::unaryNOP},
            event);
}

void
Player::update(glm::dvec2 const& terrainOffset, double dt)
{
    position += dt * m_scale * glm::rotateY(m_velocity, lookAtOffset.x);
    if(m_autoZoom) {
        m_scale = position.y;
    }
    else {
        m_scale *= std::exp(m_scaleVelocity * dt);
    }

    auto dPos       = terrainOffset - m_terrainOffset;
    m_terrainOffset = terrainOffset;
    position.x -= dPos.x;
    position.z -= dPos.y;
}

void
Player::keyDown(KeyDown const key)
{
    switch(key.code) {
    case GLFW_KEY_J: {
        m_scaleVelocity += -1.0;
    } break;
    case GLFW_KEY_K: {
        m_scaleVelocity += 1.0;
    } break;
    case GLFW_KEY_O: {
        m_autoZoom = !m_autoZoom;
    } break;
    }
}

void
Player::keyUp(KeyUp const key)
{
    switch(key.code) {
    case GLFW_KEY_J: {
        m_scaleVelocity += 1.0;
    } break;
    case GLFW_KEY_K: {
        m_scaleVelocity += -1.0;
    } break;
    }
}
