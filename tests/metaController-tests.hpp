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
auto constexpr switchKey = GLFW_KEY_C;

template<int n>
class Controller final : public PlayerController {
public:
    auto
    handleEvent(Event const& event) -> void final
    {
        if(std::holds_alternative<KeyDown>(event)
           && std::get<KeyDown>(event).code == switchKey) {
            ++switchCalls[n];
        }
    }

    auto
    update(Player&, double) -> void final
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

    auto player = Player();

    meta.handleEvent(KeyDown{switchKey});
    REQUIRE(switchCalls == std::array{0, 1, 0});
    meta.update(player, 0.0);
    REQUIRE(updateCalled == std::array{false, true, false});

    meta.handleEvent(KeyDown{switchKey});
    REQUIRE(switchCalls == std::array{0, 1, 1});
    meta.update(player, 0.0);
    REQUIRE(updateCalled == std::array{false, true, true});

    meta.handleEvent(KeyDown{switchKey});
    REQUIRE(switchCalls == std::array{1, 1, 1});
    meta.update(player, 0.0);
    REQUIRE(updateCalled == std::array{true, true, true});
}

}    // namespace MetaControllerTests

#endif    // MANDELLANDSCAPE_META_CONTROLLER_TESTS_HPP
