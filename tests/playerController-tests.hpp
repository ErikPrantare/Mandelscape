#ifndef MANDELLANDSCAPE_PLAYERCONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_PLAYERCONTROLLER_TESTS_HPP

#include <catch2/catch.hpp>
#include <memory>

#include "event.hpp"
#include "player.hpp"
#include "testUtils.hpp"

#include "playerController.hpp"

namespace PlayerControllerTest {

glm::dvec3 constexpr dummyInit = {1.23, 6532936528529.363663, -0.00001};
glm::dvec3 constexpr postEvent = {666666666, -0.363663, -1.0 / 3.0};

class DummyController : public PlayerController {
public:
    auto
    handleEvent(Event const&) -> void override
    {
        m_val = postEvent;
    }

    auto
    update(Player* player, double /*dt*/) -> void override
    {
        player->position = m_val;
    }

private:
    glm::dvec3 m_val = dummyInit;
};

TEST_CASE("PlayerController can mutate a player", "[PlayerController]")
{
    auto controller = std::unique_ptr<PlayerController>(new DummyController());
    auto player     = Player();

    controller->update(&player, 1.0);
    REQUIRE(player.position == dummyInit);

    controller->handleEvent(MouseButtonDown{});
    controller->update(&player, 1.0);

    REQUIRE(player.position == postEvent);
}

auto
movementDependentOnScaleTest(
        std::unique_ptr<PlayerController>& controller,
        Player player,
        double scaleFactor) -> void
{
    auto firstPos = player.position;
    controller->update(&player, 1.0);
    auto dNormalPos = player.position - firstPos;

    player.position = firstPos;
    player.scale *= scaleFactor;
    controller->update(&player, 1.0);
    auto dScaledPos = player.position - firstPos;
    REQUIRE(scaleFactor * dNormalPos == Dvec3Approx{dScaledPos});
}

}    // namespace PlayerControllerTest

#endif
