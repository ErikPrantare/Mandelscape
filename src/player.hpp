#ifndef MANDELLANDSCAPE_PLAYER_HPP
#define MANDELLANDSCAPE_PLAYER_HPP

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "event.hpp"
#include "utils.hpp"

class Player {
public:
    void
    handleEvent(Event event);

    void
    update(glm::dvec2 const& terrainOffset, double dt);

    glm::dvec3
    absolutePosition() const
    {
        return position + glm::dvec3(m_terrainOffset.x, 0.0, m_terrainOffset.y);
    }

    glm::dvec3
    relativePosition() const
    {
        return position;
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

    void
    setHeight(double y)
    {
        position.y = y;
    }

    glm::dvec3 position     = glm::dvec3(0, 0, 0);
    glm::dvec2 lookAtOffset = glm::dvec2(0.0, 0.0);

private:
    glm::dvec3 m_velocity{0.0, 0.0, 0.0};
    glm::dvec2 m_terrainOffset = glm::dvec2(0, 0);
    double m_scale             = 1.0;
    double m_scaleVelocity     = 0.0;
    bool m_autoZoom            = false;

    void
    keyDown(KeyDown key);

    void
    keyUp(KeyUp key);
};

#endif
