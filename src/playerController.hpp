#ifndef MANDELLANDSCAPE_PLAYER_CONTROLLER_HPP
#define MANDELLANDSCAPE_PLAYER_CONTROLLER_HPP

#include "event.hpp"
#include "player.hpp"

#include <glm/vec2.hpp>

class PlayerController {
public:
    virtual auto
    handleEvent(Event const& event) -> void = 0;

    virtual auto
    update(Player* player, double dt) -> void = 0;

    virtual ~PlayerController() = default;

    PlayerController()                        = default;
    PlayerController(PlayerController&&)      = default;
    PlayerController(PlayerController const&) = default;

    auto operator=(PlayerController &&) -> PlayerController& = default;
    auto
    operator=(PlayerController const&) -> PlayerController& = default;
};

#endif    // MANDELLANDSCAPE_PLAYER_CONTROLLER_HPP
