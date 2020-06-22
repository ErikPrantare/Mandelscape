#ifndef MANDELLANDSCAPE_AUTO_CONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_AUTO_CONTROLLER_TESTS_HPP

#include <catch2/catch.hpp>

#include "playerController.hpp"

#include "autoController.hpp"

namespace AutoControllerTest {

TEST_CASE("AutoController moves player")
{
    auto controller = std::unique_ptr<PlayerController>(new AutoController);
    auto player     = Player();

    auto prevPos = player.position;
    controller->update(&player, 1.0);
    REQUIRE(player.position != prevPos);

    SECTION("Movement is dependent on player scale")
    {
        PlayerControllerTest::movementDependentOnScaleTest(
                controller,
                player,
                32.888);
    }
}

}    // namespace AutoControllerTest

#endif
