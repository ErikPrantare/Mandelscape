#ifndef MANDELLANDSCAPE_PLAYERCONTROLLER_HPP
#define MANDELLANDSCAPE_PLAYERCONTROLLER_HPP

#include <glm/glm.hpp>

#include "event.hpp"

class PlayerController {
public:
    virtual ~PlayerController()                    = default;
    auto virtual velocity() const -> glm::dvec3    = 0;
    auto virtual handleEvent(Event const&) -> void = 0;
};

#endif
