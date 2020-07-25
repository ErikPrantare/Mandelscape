#ifndef MANDELLANDSCAPE_WALK_CONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_WALK_CONTROLLER_TESTS_HPP

#include "event.hpp"
#include "player.hpp"
#include "utils.hpp"
#include "walkController.hpp"
#include "testUtils.hpp"

#include <catch2/catch.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <memory>

namespace WalkControllerTests {

glm::dvec3 constexpr left{-1.0, 0.0, 0.0};
glm::dvec3 constexpr right{1.0, 0.0, 0.0};
glm::dvec3 constexpr front{0.0, 0.0, -1.0};
glm::dvec3 constexpr back{0.0, 0.0, 1.0};

glm::dvec3 constexpr zero{0.0, 0.0, 0.0};

void
handleEventAndUpdate(
        WalkController& controller,
        Event const& event,
        Player& player,
        double dt)
{
    controller.handleEvent(event);
    controller.update(player, 1.0);
}

auto
movementDependentOnScaleTest(
        WalkController& controller,
        Player player,
        double scaleFactor) -> void
{
    auto firstPos = player.position;
    controller.update(player, 1.0);
    auto dNormalPos = player.position - firstPos;

    player.position = firstPos;
    player.scale *= scaleFactor;
    controller.update(player, 1.0);
    auto dScaledPos = player.position - firstPos;
    REQUIRE(scaleFactor * dNormalPos == Dvec3Approx{dScaledPos});
}

TEST_CASE("WalkController handles movement keys", "[WalkController]")
{
    auto controller = WalkController{};
    auto player     = Player();

    REQUIRE(player.position == zero);

    auto testDirection = [&controller](int key, glm::dvec3 direction) {
        auto player = Player();

        handleEventAndUpdate(controller, KeyDown{key}, player, 1.0);
        REQUIRE(player.position == direction);

        controller.update(player, 1.0);
        REQUIRE(player.position == 2.0 * direction);

        handleEventAndUpdate(controller, KeyUp{key}, player, 1.0);
        REQUIRE(player.position == 2.0 * direction);
    };

    SECTION("No buttons held -> no movement")
    {
        controller.update(player, 1.0);
        REQUIRE(player.position == zero);
    }

    SECTION("WASD moves correctly")
    {
        testDirection(GLFW_KEY_W, front);
        testDirection(GLFW_KEY_A, left);
        testDirection(GLFW_KEY_S, back);
        testDirection(GLFW_KEY_D, right);
    }

    SECTION("Combination of inputs accelerate in multiple directions")
    {
        controller.handleEvent(KeyDown{GLFW_KEY_S});
        controller.handleEvent(KeyDown{GLFW_KEY_A});
        controller.update(player, 1.0);
        REQUIRE(player.position == back + left);
    }

    SECTION("Movement in opposite directions yields no velocity")
    {
        controller.handleEvent(KeyDown{GLFW_KEY_A});
        controller.handleEvent(KeyDown{GLFW_KEY_D});
        controller.update(player, 1.0);
        REQUIRE(player.position == zero);
        controller.handleEvent(KeyDown{GLFW_KEY_W});
        controller.handleEvent(KeyDown{GLFW_KEY_S});
        controller.update(player, 1.0);
        REQUIRE(player.position == zero);
    }

    SECTION("Movement is dependent on player scale")
    {
        controller.handleEvent(KeyDown{GLFW_KEY_W});

        movementDependentOnScaleTest(controller, player, 0.6838);
    }

    SECTION("Movement is dependent on player look direction")
    {
        auto rotation         = -46.0997;
        player.lookAtOffset.x = rotation;

        controller.handleEvent(KeyDown{GLFW_KEY_W});
        controller.update(player, 1.0);

        auto rotator = glm::rotate(glm::dmat4(1.0), rotation, {0.0, 1.0, 0.0});
        auto dPos    = rotator * glm::dvec4(front, 0.0);

        REQUIRE(player.position == Dvec3Approx{dPos});
    }
}

TEST_CASE("WalkController handles mouse movement", "[WalkController]")
{
    auto controller = WalkController{};
    auto player     = Player();

    auto constexpr dontCare = 0.0;

    auto dMovement = [](glm::dvec2 dPos) {
        return MouseMove{0.0, 0.0, dPos.x, dPos.y};
    };

    auto directionTest = [&](glm::dvec2 pixelsMoved) {
        auto const angleMoved = util::pixelsToAngle(pixelsMoved);

        controller.handleEvent(dMovement(pixelsMoved));
        controller.update(player, dontCare);
        REQUIRE(player.lookAtOffset == angleMoved);

        controller.handleEvent(dMovement(-pixelsMoved));
        controller.update(player, dontCare);
        REQUIRE(player.lookAtOffset == glm::dvec2{0.0, 0.0});

        controller.handleEvent(dMovement(-pixelsMoved));
        controller.update(player, dontCare);
        REQUIRE(player.lookAtOffset == -angleMoved);
    };

    REQUIRE(player.position == zero);

    SECTION("Mouse movement in vertical direction")
    {
        directionTest({0.0, 23.0});
    }

    SECTION("Mouse movement in horizontal direction")
    {
        directionTest({-177.0, 0.0});
    }

    SECTION("Mouse movement in diagonal direction")
    {
        directionTest(glm::dvec2{230.0, -63.0});
    }

    SECTION("Multiple mouse events before update")
    {
        auto constexpr pixelsMoved = 73;
        auto constexpr angleMoved  = util::pixelsToAngle({pixelsMoved, 0.0});

        controller.handleEvent(dMovement({pixelsMoved, 0}));
        controller.handleEvent(dMovement({pixelsMoved, 0}));
        controller.update(player, dontCare);
        REQUIRE(player.lookAtOffset == 2.0 * angleMoved);
    }

    SECTION("Vertical look direction does not exceed +-pi/2")
    {
        auto constexpr manyPixels = 9999999999;

        controller.handleEvent(dMovement({0, manyPixels}));
        controller.update(player, dontCare);
        REQUIRE(player.lookAtOffset.y < glm::pi<double>() / 2.0);

        controller.handleEvent(dMovement({0, -manyPixels}));
        controller.update(player, dontCare);
        REQUIRE(player.lookAtOffset.y > -glm::pi<double>() / 2);
    }
}

TEST_CASE("WalkController controlls player scale", "[WalkController]")
{
    auto controller = WalkController{};
    auto player     = Player();
    auto dontCare   = 0.0;

    controller.update(player, 1.0);
    REQUIRE(player.scale == 1.0);

    SECTION("J shrinks player scale")
    {
        controller.handleEvent(KeyDown{GLFW_KEY_J});
        controller.update(player, 1.0);
        auto scale = player.scale;
        REQUIRE(scale < 1.0);

        controller.handleEvent(KeyUp{GLFW_KEY_J});
        controller.update(player, 1.0);
        REQUIRE(player.scale == scale);
    }

    SECTION("K grows player scale")
    {
        controller.handleEvent(KeyDown{GLFW_KEY_K});
        controller.update(player, 1.0);
        auto scale = player.scale;
        REQUIRE(scale > 1.0);

        controller.handleEvent(KeyUp{GLFW_KEY_K});
        controller.update(player, 1.0);
        REQUIRE(player.scale == scale);
    }

    SECTION("Holding both J and K does nothing")
    {
        controller.handleEvent(KeyDown{GLFW_KEY_K});
        controller.handleEvent(KeyDown{GLFW_KEY_J});
        controller.update(player, 1.0);
        REQUIRE(player.scale == 1.0);
    }

    SECTION("O enables autozoom")
    {
        auto height       = 0.200468;
        player.position.y = height;
        controller.handleEvent(KeyDown{GLFW_KEY_O});
        controller.update(player, dontCare);
        REQUIRE(player.scale == height);
    }

    SECTION("Scaling happens exponentially")
    {
        auto dt = 3.60887;
        controller.handleEvent(KeyDown{GLFW_KEY_J});
        auto playerReference = Player();
        controller.update(playerReference, dt);
        auto scaleReference = playerReference.scale;

        SECTION("Double the delta time squares the scale change")
        {
            controller.update(player, 2.0 * dt);
            REQUIRE(scaleReference * scaleReference == Approx(player.scale));
        }

        SECTION("Twice updating squares the scale change")
        {
            controller.update(player, dt);
            controller.update(player, dt);
            REQUIRE(scaleReference * scaleReference == Approx(player.scale));
        }
    }
}

}    // namespace WalkControllerTests

#endif
