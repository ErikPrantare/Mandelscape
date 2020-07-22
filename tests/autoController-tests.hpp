#ifndef MANDELLANDSCAPE_AUTO_CONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_AUTO_CONTROLLER_TESTS_HPP

#include "autoController.hpp"

#include <catch2/catch.hpp>

namespace AutoControllerTests {

TEST_CASE("AutoController moves player", "[AutoController]")
{
    auto heightFunc = [](auto x) {
        return x.x;
    };
    auto controller = AutoController(heightFunc);
    auto player     = Player();

    for(auto i = 0; i < 10; ++i) {
        auto prevPos = player.position;
        controller.update(player, 1.0);
        REQUIRE(player.position != prevPos);
    }
}

}    // namespace AutoControllerTests

#endif
