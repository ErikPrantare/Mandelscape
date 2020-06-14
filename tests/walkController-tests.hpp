#ifndef MANDELLANDSCAPE_WALK_CONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_WALK_CONTROLLER_TESTS_HPP

#include <catch2/catch.hpp>
#include <memory>

#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>

#include "event.hpp"
#include "player.hpp"

#include "utils.hpp"
#include "walkController.hpp"

namespace WalkControllerTests {

glm::dvec3 constexpr left{-1.0, 0.0, 0.0};
glm::dvec3 constexpr right{1.0, 0.0, 0.0};
glm::dvec3 constexpr front{0.0, 0.0, -1.0};
glm::dvec3 constexpr back{0.0, 0.0, 1.0};

glm::dvec3 constexpr zero{0.0, 0.0, 0.0};

void
handleEventAndUpdate(
        std::unique_ptr<PlayerController>& controller,
        Event const& event,
        Player* player,
        double dt)
{
    controller->handleEvent(event);
    controller->update(player, 1.0);
}

TEST_CASE("WalkController handles movement keys", "[WalkController]")
{
    auto controller = std::unique_ptr<PlayerController>(new WalkController);
    auto player     = Player();

    SECTION("No buttons held -> no movement")
    {
        controller->update(&player, 1.0);
        REQUIRE(player.position == zero);
    }

    SECTION("\"A\" button hold moves left")
    {
        handleEventAndUpdate(controller, KeyDown{GLFW_KEY_A}, &player, 1.0);
        REQUIRE(player.position == left);

        controller->update(&player, 1.0);
        REQUIRE(player.position == 2.0 * left);

        handleEventAndUpdate(controller, KeyUp{GLFW_KEY_A}, &player, 1.0);
        REQUIRE(player.position == 2.0 * left);
    }
    SECTION("\"D\" button hold moves right")
    {
        handleEventAndUpdate(controller, KeyDown{GLFW_KEY_D}, &player, 1.0);
        REQUIRE(player.position == right);

        controller->update(&player, 1.0);
        REQUIRE(player.position == 2.0 * right);

        handleEventAndUpdate(controller, KeyUp{GLFW_KEY_D}, &player, 1.0);
        REQUIRE(player.position == 2.0 * right);
    }
    SECTION("\"W\" button hold moves forwards")
    {
        handleEventAndUpdate(controller, KeyDown{GLFW_KEY_W}, &player, 1.0);
        REQUIRE(player.position == front);

        controller->update(&player, 1.0);
        REQUIRE(player.position == 2.0 * front);

        handleEventAndUpdate(controller, KeyUp{GLFW_KEY_W}, &player, 1.0);
        REQUIRE(player.position == 2.0 * front);
    }
    SECTION("\"S\" button hold moves backwards")
    {
        handleEventAndUpdate(controller, KeyDown{GLFW_KEY_S}, &player, 1.0);
        REQUIRE(player.position == back);

        controller->update(&player, 1.0);
        REQUIRE(player.position == 2.0 * back);

        handleEventAndUpdate(controller, KeyUp{GLFW_KEY_S}, &player, 1.0);
        REQUIRE(player.position == 2.0 * back);
    }

    SECTION("Combination of inputs accelerate in multiple directions")
    {
        controller->handleEvent(KeyDown{GLFW_KEY_S});
        controller->handleEvent(KeyDown{GLFW_KEY_A});
        controller->update(&player, 1.0);
        REQUIRE(player.position == back + left);
    }

    SECTION("Movement in opposite directions yields no velocity")
    {
        controller->handleEvent(KeyDown{GLFW_KEY_A});
        controller->handleEvent(KeyDown{GLFW_KEY_D});
        controller->update(&player, 1.0);
        REQUIRE(player.position == zero);
        controller->handleEvent(KeyDown{GLFW_KEY_W});
        controller->handleEvent(KeyDown{GLFW_KEY_S});
        controller->update(&player, 1.0);
        REQUIRE(player.position == zero);
    }
}

TEST_CASE("WalkController handles mouse movement", "[WalkController]")
{
    auto controller = std::unique_ptr<PlayerController>(new WalkController);
    auto player     = Player();

    auto constexpr dontCare = 0.0;

    auto dMovement = [](double dx, double dy) {
        return MouseMove{0.0, 0.0, dx, dy};
    };

    REQUIRE(player.position == zero);

    SECTION("Mouse movement in vertical direction")
    {
        auto constexpr pixelsMoved = 23.0;

        controller->handleEvent(dMovement(0.0, pixelsMoved));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset.y == util::pixelsToAngle(pixelsMoved));

        controller->handleEvent(dMovement(0.0, -pixelsMoved));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset.y == 0.0);
        controller->handleEvent(dMovement(0.0, -pixelsMoved));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset.y == -util::pixelsToAngle(pixelsMoved));
    }

    SECTION("Mouse movement in horizontal direction")
    {
        auto constexpr pixelsMoved = -177.0;

        controller->handleEvent(dMovement(pixelsMoved, 0.0));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset.x == util::pixelsToAngle(pixelsMoved));

        controller->handleEvent(dMovement(-pixelsMoved, 0.0));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset.x == 0.0);
        controller->handleEvent(dMovement(-pixelsMoved, 0.0));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset.x == -util::pixelsToAngle(pixelsMoved));
    }

    SECTION("Mouse movement in diagonal direction")
    {
        auto constexpr pixelsMoved = glm::dvec2(230.0, -63.0);

        controller->handleEvent(dMovement(pixelsMoved.x, pixelsMoved.y));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset.x == util::pixelsToAngle(pixelsMoved.x));
        REQUIRE(player.lookAtOffset.y == util::pixelsToAngle(pixelsMoved.y));

        controller->handleEvent(dMovement(-pixelsMoved.x, -pixelsMoved.y));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset == glm::dvec2{0.0, 0.0});

        controller->handleEvent(dMovement(-pixelsMoved.x, -pixelsMoved.y));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset.x == -util::pixelsToAngle(pixelsMoved.x));
        REQUIRE(player.lookAtOffset.y == -util::pixelsToAngle(pixelsMoved.y));
    }

    SECTION("Multiple mouse events before update")
    {
        auto constexpr pixelsMoved = 73;

        controller->handleEvent(dMovement(pixelsMoved, 0));
        controller->handleEvent(dMovement(pixelsMoved, 0));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset.x
                == 2.0 * util::pixelsToAngle(pixelsMoved));
    }

    SECTION("Vertical look direction does not exceed +-pi/2")
    {
        auto constexpr manyPixels = 9999999999;

        controller->handleEvent(dMovement(0, manyPixels));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset.y < glm::pi<double>() / 2.0);

        controller->handleEvent(dMovement(0, -manyPixels));
        controller->update(&player, dontCare);
        REQUIRE(player.lookAtOffset.y > -glm::pi<double>() / 2);
    }
}
}    // namespace WalkControllerTests

#endif
