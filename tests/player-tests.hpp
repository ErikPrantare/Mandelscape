#ifndef MANDELLANDSCAPE_PLAYER_TESTS_HPP
#define MANDELLANDSCAPE_PLAYER_TESTS_HPP

#include "utils.hpp"

#include <catch2/catch.hpp>

#include "event.hpp"

#include "player.hpp"

namespace PlayerTest {

glm::dvec3 constexpr G_VELOCITY{123.33, -44.44, 2.5};

class DummyController : public PlayerController {
public:
    auto
    velocity() const -> glm::dvec3 final
    {
        return G_VELOCITY;
    }

    auto
    handleEvent(Event const&) -> void final
    {}
};

auto
dvecApproxEqual(glm::dvec3 v, glm::dvec3 approx) -> bool
{
    return v.x == Approx(approx.x) && v.y == Approx(approx.y)
           && v.z == Approx(approx.z);
}

TEST_CASE(
        "Player moves in the direction mandated by the controller",
        "[Player]")
{
    auto player = Player(util::tag<DummyController>());
    player.update({0, 0}, 1.0);

    REQUIRE(dvecApproxEqual(player.absolutePosition(), G_VELOCITY));
}

}    // namespace PlayerTest

#endif
