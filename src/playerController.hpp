#ifndef MANDELLANDSCAPE_PLAYER_CONTROLLER_HPP
#define MANDELLANDSCAPE_PLAYER_CONTROLLER_HPP

#include <glm/vec2.hpp>

#include "event.hpp"
#include "player.hpp"
#include "momentaryAction.hpp"
#include "persistentActionMap.hpp"

class PlayerController {
public:
    virtual auto
    handleMomentaryAction(MomentaryAction const& action) -> void = 0;

    virtual auto
    updateState(PersistentActionMap const& map) -> void = 0;

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
