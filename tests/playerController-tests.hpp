#ifndef MANDELLANDSCAPE_PLAYER_CONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_PLAYER_CONTROLLER_TESTS_HPP

#include "event.hpp"
#include "player.hpp"
#include "playerController.hpp"

#include <catch2/catch.hpp>

#include <functional>

namespace PlayerControllerTests {

auto eventHandled = false;
auto updateCalled = false;

struct Controller : PlayerController {
    auto
    handleEvent(Event const&) -> void final
    {
        eventHandled = true;
    }

    auto
    update(Player&, double) -> void final
    {
        updateCalled = true;
    }
};

TEST_CASE(
        "PlayerController calls derived controllers implementations",
        "[PlayerController]")
{
    auto playerController = std::unique_ptr<PlayerController>{new Controller};

    auto const dummyEvent = Event{KeyDown{}};
    playerController->handleEvent(dummyEvent);
    REQUIRE(eventHandled);

    auto dummyPlayer = Player{};
    playerController->update(dummyPlayer, {});
    REQUIRE(updateCalled);
}

}    // namespace PlayerControllerTests

#endif    // MANDELLANDSCAPE_PLAYER_CONTROLLER_TESTS_HPP
