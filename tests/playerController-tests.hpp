#ifndef MANDELLANDSCAPE_PLAYER_CONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_PLAYER_CONTROLLER_TESTS_HPP

#include "event.hpp"
#include "player.hpp"
#include "playerController.hpp"

#include <catch2/catch.hpp>

#include <functional>

namespace PlayerControllerTests {

struct Controller : PlayerController {
    std::reference_wrapper<bool> handleEventFlag;
    std::reference_wrapper<bool> updateFlag;

    Controller(bool& _handleEventFlag, bool& _updateFlag) :
                handleEventFlag{_handleEventFlag},
                updateFlag{_updateFlag}
    {
        REQUIRE(handleEventFlag.get() == false);
        REQUIRE(updateFlag.get() == false);
    }

    auto
    handleEvent(Event const&) -> void final
    {
        handleEventFlag.get() = !handleEventFlag.get();
    }

    auto
    update(Player&, double) -> void final
    {
        updateFlag.get() = !updateFlag.get();
    }
};

TEST_CASE(
        "PlayerController calls erased controllers implementations",
        "[PlayerController]")
{
    auto handleEventFlag = false;
    auto updateFlag      = false;

    auto playerController = std::unique_ptr<PlayerController>{
            new Controller{handleEventFlag, updateFlag}};

    SECTION("PlayerController::handleEvent")
    {
        auto const dummyEvent = Event{KeyDown{}};
        playerController->handleEvent(dummyEvent);
        REQUIRE(handleEventFlag);
        REQUIRE_FALSE(updateFlag);
    }

    SECTION("PlayerController::update")
    {
        auto dummyPlayer = Player{};
        playerController->update(dummyPlayer, {});
        REQUIRE_FALSE(handleEventFlag);
        REQUIRE(updateFlag);
    }
}

}    // namespace PlayerControllerTests

#endif    // MANDELLANDSCAPE_PLAYER_CONTROLLER_TESTS_HPP
