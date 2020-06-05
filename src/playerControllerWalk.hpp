#ifndef MANDELLANDSCAPE_PLAYERCONTROLLERWALK_HPP
#define MANDELLANDSCAPE_PLAYERCONTROLLERWALK_HPP

#include <glm/glm.hpp>

#include "event.hpp"
#include "playerController.hpp"

class PlayerControllerWalk : public PlayerController {
public:
    glm::dvec3
    velocity() const override;

    void
    handleEvent(Event const&) override;

private:
    glm::dvec3 m_velocity{0.0, 0.0, 0.0};
};

#endif
