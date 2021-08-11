/* Mandelscape
 * Copyright (C) 2020-2021 Erik Präntare
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <catch2/catch.hpp>

#include <array>
#include <variant>

#include <catch2/catch.hpp>

#include "metaController.hpp"
#include "event.hpp"
#include "player.hpp"

auto switchCalls         = std::array{0, 0, 0};
auto updateCalled        = std::array{false, false, false};
auto stateUpdateCalled   = std::array{false, false, false};
auto constexpr switchKey = Input::Key::C;

template<int n>
class Controller final : public Player::Controller {
public:
    auto
    handleMomentaryAction(MomentaryAction const& action) -> void final
    {
        if(std::holds_alternative<Trigger>(action)
           && std::get<Trigger>(action) == Trigger::ToggleAutoWalk) {
            ++switchCalls[n];
        }
    }

    auto
    updateState(StateMap const& /*map*/) -> void final
    {
        stateUpdateCalled[n] = true;
    }

    auto
    update(Player::Internals&, double) -> void final
    {
        updateCalled[n] = true;
    }
};

TEST_CASE("MetaController switches controllers", "[MetaController]")
{
    auto meta = MetaController(
            std::make_unique<Controller<0>>(),
            std::make_unique<Controller<1>>(),
            std::make_unique<Controller<2>>());

    auto player              = Player();
    auto const persistentMap = StateMap{};

    meta.handleMomentaryAction(Trigger::ToggleAutoWalk);
    meta.updateState(persistentMap);
    REQUIRE(switchCalls == std::array{0, 1, 0});
    meta.update(player, 0.0);
    REQUIRE(updateCalled == std::array{false, true, false});
    REQUIRE(stateUpdateCalled == std::array{false, true, false});

    meta.handleMomentaryAction(Trigger::ToggleAutoWalk);
    meta.updateState(persistentMap);
    REQUIRE(switchCalls == std::array{0, 1, 1});
    meta.update(player, 0.0);
    REQUIRE(updateCalled == std::array{false, true, true});
    REQUIRE(stateUpdateCalled == std::array{false, true, true});

    meta.handleMomentaryAction(Trigger::ToggleAutoWalk);
    meta.updateState(persistentMap);
    REQUIRE(switchCalls == std::array{1, 1, 1});
    meta.update(player, 0.0);
    REQUIRE(updateCalled == std::array{true, true, true});
    REQUIRE(stateUpdateCalled == std::array{true, true, true});
}
