#ifndef MANDELLANDSCAPE_PLAYERCONTROLLER_HPP
#define MANDELLANDSCAPE_PLAYERCONTROLLER_HPP

#include <glm/glm.hpp>

#include "event.hpp"
#include "player.hpp"

class PlayerController {
public:
    PlayerController()                              = default;
    PlayerController(PlayerController const& other) = default;
    PlayerController&
    operator=(PlayerController const& other)   = default;
    PlayerController(PlayerController&& other) = default;
    PlayerController&
    operator=(PlayerController&& other) = default;

    virtual ~PlayerController() = default;

    auto virtual handleEvent(Event const&) -> void = 0;
    auto virtual update(Player*, double) -> void   = 0;
};

#endif
