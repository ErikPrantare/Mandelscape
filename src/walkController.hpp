#ifndef MANDELLANDSCAPE_WALKCONTROLLER_HPP
#    define MANDELLANDSCAPE_WALKPLAYERCONTROLLER_HPP

#    include <glm/glm.hpp>

#    include "event.hpp"
#    include "playerController.hpp"

class WalkController : public PlayerController {
public:
    glm::dvec3
    velocity() const override;

    void
    handleEvent(Event const&) override;

private:
    static double constexpr m_movementSpeed = 1.0;
    glm::dvec3 m_direction{0.0, 0.0, 0.0};
};

#endif
