#ifndef MANDELLANDSCAPE_PLAYER_CONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_PLAYER_CONTROLLER_TESTS_HPP

#include "playerController.hpp"

#include <catch2/catch.hpp>

#include <functional>

struct Controller {
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
    handleEvent(Event const&) -> void
    {
        handleEventFlag.get() = !handleEventFlag.get();
    }

    auto
    update(Player&, double) -> void
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

    auto playerController =
            PlayerController{Controller{handleEventFlag, updateFlag}};

    SECTION("PlayerController::handleEvent")
    {
        auto const dummyEvent = Event{KeyDown{}};
        playerController.handleEvent(dummyEvent);
        REQUIRE(handleEventFlag);
        REQUIRE_FALSE(updateFlag);
    }

    SECTION("PlayerController::update")
    {
        auto dummyPlayer = Player{};
        playerController.update(dummyPlayer, {});
        REQUIRE_FALSE(handleEventFlag);
        REQUIRE(updateFlag);
    }
}

#endif    // MANDELLANDSCAPE_PLAYER_CONTROLLER_TESTS_HPP
