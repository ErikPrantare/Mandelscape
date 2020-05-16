#include "player.h"

#include <algorithm>
#include <functional>

#include <glm/gtx/rotate_vector.hpp>
#include <GLFW/glfw3.h>

#include "event.h"
#include "utils.h"
#include "mandelTypeTraits.h"

void
Player::handleEvent(Event const event)
{
    // CPP20
    // https://en.cppreference.com/w/cpp/utility/functional/bind_front

    std::visit(
            util::Overload{
                    [this](KeyDown keyEvent) { keyDown(keyEvent); },
                    [this](KeyUp keyEvent) { keyUp(keyEvent); },
                    [this](MouseMove mouseEvent) { mouseMove(mouseEvent); },
                    // default
                    [](auto x) {
                    }},
            event);
}

void
Player::update(glm::vec2 terrainOffset, double dt)
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

void
Player::keyDown(KeyDown const key)
{
    switch(key.code) {
    case GLFW_KEY_W: {
        m_velocity.z += -movementSpeed;
    } break;
    case GLFW_KEY_A: {
        m_velocity.x += -movementSpeed;
    } break;
    case GLFW_KEY_S: {
        m_velocity.z += movementSpeed;
    } break;
    case GLFW_KEY_D: {
        m_velocity.x += movementSpeed;
    } break;
    case GLFW_KEY_J: {
        m_scaleVelocity += -1.f;
    } break;
    case GLFW_KEY_K: {
        m_scaleVelocity += 1.f;
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
    case GLFW_KEY_W: {
        m_velocity.z += movementSpeed;
    } break;
    case GLFW_KEY_A: {
        m_velocity.x += movementSpeed;
    } break;
    case GLFW_KEY_S: {
        m_velocity.z += -movementSpeed;
    } break;
    case GLFW_KEY_D: {
        m_velocity.x += -movementSpeed;
    } break;
    case GLFW_KEY_J: {
        m_scaleVelocity += 1.f;
    } break;
    case GLFW_KEY_K: {
        m_scaleVelocity += -1.f;
    } break;
    }
}

void
Player::mouseMove(MouseMove mouse)
{
    m_lookAtOffset.x += mouse.dx / 100.f;
    m_lookAtOffset.y += mouse.dy / 100.f;
    m_lookAtOffset.y = std::clamp(
            m_lookAtOffset.y,
            float(-glm::pi<double>() / 2 + 0.001),
            float(glm::pi<double>() / 2 - 0.001));
};
