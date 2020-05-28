#ifndef MANDELLANDSCAPE_PLAYER_H
#define MANDELLANDSCAPE_PLAYER_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "event.hpp"

class Player {
public:
    void
    handleEvent(Event event);

    void
    update(glm::dvec2 const& terrainOffset, double dt);

    glm::dvec3
    absolutePosition() const
    {
        return m_position
               + glm::dvec3(m_terrainOffset.x, 0.0, m_terrainOffset.y);
    }

    glm::dvec3
    relativePosition() const
    {
        return m_position;
    }

    glm::dvec2
    terrainOffset() const
    {
        return m_terrainOffset;
    }

    double
    scale() const
    {
        return m_scale;
    }

    glm::dvec2
    lookAtOffset() const
    {
        return m_lookAtOffset;
    }

    void
    setHeight(double y)
    {
        m_position.y = y;
    }

private:
    glm::dvec3 m_position      = glm::dvec3(0, 0, 0);
    glm::dvec3 m_velocity      = glm::dvec3(0, 0, 0);
    glm::dvec2 m_terrainOffset = glm::dvec2(0, 0);
    double m_scale             = 1.0;
    double m_scaleVelocity     = 0.0;
    bool m_autoZoom            = false;

    glm::dvec2 m_lookAtOffset = glm::dvec2(0.0, 0.0);

    static double constexpr movementSpeed = 1.0;

    void
    keyDown(KeyDown key);

    void
    keyUp(KeyUp key);

    void
    mouseMove(MouseMove mouse);
};

#endif
