#ifndef MANDELLANDSCAPE_META_CONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_META_CONTROLLER_TESTS_HPP

#include <array>
#include <variant>

#include <catch2/catch.hpp>

#include "metaController.hpp"
#include "playerController.hpp"
#include "event.hpp"
#include "player.hpp"

namespace MetaControllerTests {

auto switchCalls         = std::array{0, 0, 0};
auto updateCalled        = std::array{false, false, false};
auto stateUpdateCalled   = std::array{false, false, false};
auto constexpr switchKey = Input::Key::C;

template<int n>
class Controller final : public PlayerController {
public:
    auto
    handleMomentaryAction(MomentaryAction const& action) -> void final
    {
        if(std::holds_alternative<TriggerAction>(action)
           && std::get<TriggerAction>(action)
                      == TriggerAction::ToggleAutoWalk) {
            ++switchCalls[n];
        }
    }

    auto
    updateState(PersistentActionMap const& map) -> void final
    {
        stateUpdateCalled[n] = true;
    }

    auto
    update(Player*, double) -> void final
    {
        updateCalled[n] = true;
    };
};

TEST_CASE("MetaController switches controllers", "[MetaController]")
{
    auto meta = MetaController(
            std::make_unique<Controller<0>>(),
            std::make_unique<Controller<1>>(),
            std::make_unique<Controller<2>>());

    auto player              = Player();
    auto const persistentMap = PersistentActionMap{};

    meta.handleMomentaryAction(TriggerAction::ToggleAutoWalk);
    meta.updateState(persistentMap);
    REQUIRE(switchCalls == std::array{0, 1, 0});
    meta.update(&player, 0.0);
    REQUIRE(updateCalled == std::array{false, true, false});
    REQUIRE(stateUpdateCalled == std::array{false, true, false});

    meta.handleMomentaryAction(TriggerAction::ToggleAutoWalk);
    meta.updateState(persistentMap);
    REQUIRE(switchCalls == std::array{0, 1, 1});
    meta.update(&player, 0.0);
    REQUIRE(updateCalled == std::array{false, true, true});
    REQUIRE(stateUpdateCalled == std::array{false, true, true});

    meta.handleMomentaryAction(TriggerAction::ToggleAutoWalk);
    meta.updateState(persistentMap);
    REQUIRE(switchCalls == std::array{1, 1, 1});
    meta.update(&player, 0.0);
    REQUIRE(updateCalled == std::array{true, true, true});
    REQUIRE(stateUpdateCalled == std::array{true, true, true});
}

}    // namespace MetaControllerTests

#endif    // MANDELLANDSCAPE_META_CONTROLLER_TESTS_HPP
