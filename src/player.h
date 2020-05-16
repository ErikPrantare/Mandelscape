#ifndef MANDELLANDSCAPE_PLAYER_H
#define MANDELLANDSCAPE_PLAYER_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "event.h"

class Player {
public:
    void
    handleEvent(Event event);

    void
    update(glm::vec2 terrainOffset, double dt);

    glm::vec3
    absolutePosition() const
    {
        return m_position
               + glm::vec3(m_terrainOffset.x, 0.f, m_terrainOffset.y);
    }

    glm::vec2
    terrainOffset() const
    {
        return m_terrainOffset;
    }

    double
    scale() const
    {
        return m_scale;
    }

    glm::vec2
    lookAtOffset() const
    {
        return m_lookAtOffset;
    }

    glm::vec3 m_position = glm::vec3(0, 0, 0);

private:
    glm::vec3 m_velocity      = glm::vec3(0, 0, 0);
    glm::vec2 m_terrainOffset = glm::vec2(0, 0);
    double m_scale            = 1.0;
    double m_scaleVelocity    = 0.0;
    bool m_autoZoom           = false;

    glm::vec2 m_lookAtOffset = glm::vec2(0.0, 0.0);

    static constexpr float movementSpeed = 1.0;

    void
    keyDown(KeyDown key);

    void
    keyUp(KeyUp key);

    void
    mouseMove(MouseMove mouse);
};

#endif
