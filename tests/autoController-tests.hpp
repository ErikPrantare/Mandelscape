#ifndef MANDELLANDSCAPE_AUTO_CONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_AUTO_CONTROLLER_TESTS_HPP

#include <catch2/catch.hpp>

#include "playerController.hpp"

#include "autoController.hpp"

namespace AutoControllerTest {

TEST_CASE("AutoController moves player", "[AutoController]")
{
    auto heightFunc = [](auto x) {
        return x.x;
    };
    auto controller =
            std::unique_ptr<PlayerController>(new AutoController(heightFunc));
    auto player = Player();

    auto prevPos = player.position;
    controller->update(&player, 1.0);
    REQUIRE(player.position != prevPos);
}

TEST_CASE(
        "AutoController moves downwards relative to terrain",
        "[AutoController]")
{
    auto player = Player();

    auto heightFunc = [](glm::dvec2 pos) {
        return std::abs(pos.x * pos.y);
    };
    auto controller =
            std::unique_ptr<PlayerController>(new AutoController(heightFunc));

    auto startPos = glm::dvec2{12.0, 47.0};

    for(double t = 0.0; t < 2.0 * 3.14; t += 0.02) {
        player.position.x = startPos.x;
        player.position.z = startPos.y;
        controller->update(&player, 1.0);
        REQUIRE(heightFunc({player.position.x, player.position.z})
                < heightFunc(startPos));
    }
}

}    // namespace AutoControllerTest

#endif
