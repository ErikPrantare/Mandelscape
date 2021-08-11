/* Mandelscape
 * Copyright (C) 2020-2021 Erik Präntare
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <catch2/catch.hpp>

#include <memory>

#include <catch2/catch.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "event.hpp"
#include "player.hpp"
#include "util.hpp"
#include "walkController.hpp"
#include "testUtils.hpp"
#include "momentaryActionsMap.hpp"

glm::dvec3 constexpr left{-1.0, 0.0, 0.0};
glm::dvec3 constexpr right{1.0, 0.0, 0.0};
glm::dvec3 constexpr front{0.0, 0.0, -1.0};
glm::dvec3 constexpr back{0.0, 0.0, 1.0};

glm::dvec3 constexpr zero{0.0, 0.0, 0.0};

TEST_CASE("WalkController handles movement keys", "[WalkController]")
{
    using namespace Input;
    auto constexpr frontKey = Key::W;
    auto constexpr backKey  = Key::S;
    auto constexpr leftKey  = Key::A;
    auto constexpr rightKey = Key::D;

    auto controller = WalkController{};
    StateMap persistentMap;
    persistentMap.add({frontKey}, State::MovingForwards);
    persistentMap.add({backKey}, State::MovingBackwards);
    persistentMap.add({leftKey}, State::MovingLeft);
    persistentMap.add({rightKey}, State::MovingRight);

    auto player = Player();

    REQUIRE(player.state().position == zero);

    SECTION("No buttons held -> no movement")
    {
        controller.update(player.state(), 1.0);
        REQUIRE(player.state().position == zero);
    }

    SECTION("Player walks in base directions correctly")
    {
        auto testDirection = [&controller,
                              &persistentMap](Key key, glm::dvec3 direction) {
            auto player = Player();

            persistentMap.updateState(KeyDown{key});
            controller.updateState(persistentMap);
            controller.update(player.state(), 1.0);
            REQUIRE(player.state().position == direction);

            controller.updateState(persistentMap);
            controller.update(player.state(), 1.0);
            REQUIRE(player.state().position == 2.0 * direction);

            persistentMap.updateState(KeyUp{key});
            controller.updateState(persistentMap);
            controller.update(player.state(), 1.0);
            REQUIRE(player.state().position == 2.0 * direction);
        };

        testDirection(frontKey, front);
        testDirection(leftKey, left);
        testDirection(backKey, back);
        testDirection(rightKey, right);
    }

    SECTION("Combination of inputs accelerate in multiple directions")
    {
        persistentMap.updateState(KeyDown{backKey});
        persistentMap.updateState(KeyDown{leftKey});
        controller.updateState(persistentMap);
        controller.update(player.state(), 1.0);
        REQUIRE(player.state().position == back + left);
    }

    SECTION("Movement in opposite directions yields no velocity")
    {
        persistentMap.updateState(KeyDown{leftKey});
        persistentMap.updateState(KeyDown{rightKey});
        controller.updateState(persistentMap);
        controller.update(player.state(), 1.0);
        REQUIRE(player.state().position == zero);

        persistentMap.updateState(KeyDown{backKey});
        persistentMap.updateState(KeyDown{frontKey});
        controller.updateState(persistentMap);
        controller.update(player.state(), 1.0);
        REQUIRE(player.state().position == zero);
    }

    SECTION("Movement is dependent on player scale")
    {
        persistentMap.updateState(KeyDown{leftKey});

        auto firstPos = player.state().position;
        controller.updateState(persistentMap);
        controller.update(player.state(), 1.0);
        auto dNormalPos = player.state().position - firstPos;

        double scaleFactor      = 0.6838;
        player.state().position = firstPos;
        player.state().scale *= scaleFactor;
        controller.updateState(persistentMap);
        controller.update(player.state(), 1.0);

        auto dScaledPos = player.state().position - firstPos;
        REQUIRE(scaleFactor * dNormalPos == Dvec3Approx{dScaledPos});
    }

    SECTION("Movement is dependent on player look direction")
    {
        auto rotation                 = -46.0997;
        player.state().lookAtOffset.x = rotation;

        persistentMap.updateState(KeyDown{frontKey});
        controller.updateState(persistentMap);
        controller.update(player.state(), 1.0);

        auto rotator = glm::rotate(glm::dmat4(1.0), rotation, {0.0, 1.0, 0.0});
        auto dPos    = rotator * glm::dvec4(front, 0.0);

        REQUIRE(player.state().position == Dvec3Approx{dPos});
    }
}

TEST_CASE("WalkController handles mouse movement", "[WalkController]")
{
    auto controller         = WalkController{};
    auto player             = Player();
    auto const momentaryMap = MomentaryActionsMap();

    auto constexpr dontCare = 0.0;

    auto dMovement = [](glm::dvec2 dPos) {
        return MouseMove{0.0, 0.0, dPos.x, dPos.y};
    };

    auto directionTest = [&](glm::dvec2 pixelsMoved) {
        auto const angleMoved = util::pixelsToAngle(pixelsMoved);

        auto actions = momentaryMap(dMovement(pixelsMoved));
        for(auto const& action : actions)
            controller.handleMomentaryAction(action);
        controller.update(player.state(), dontCare);
        REQUIRE(player.state().lookAtOffset == angleMoved);

        actions = momentaryMap(dMovement(-pixelsMoved));
        for(auto const& action : actions)
            controller.handleMomentaryAction(action);
        controller.update(player.state(), dontCare);
        REQUIRE(player.state().lookAtOffset == glm::dvec2{0.0, 0.0});

        actions = momentaryMap(dMovement(-pixelsMoved));
        for(auto const& action : actions)
            controller.handleMomentaryAction(action);
        controller.update(player.state(), dontCare);
        REQUIRE(player.state().lookAtOffset == -angleMoved);
    };

    REQUIRE(player.state().position == zero);

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

        auto actions = momentaryMap(dMovement({pixelsMoved, 0}));
        for(auto const& action : actions) {
            controller.handleMomentaryAction(action);
            controller.handleMomentaryAction(action);
        }
        controller.update(player.state(), dontCare);
        REQUIRE(player.state().lookAtOffset == 2.0 * angleMoved);
    }

    SECTION("Vertical look direction does not exceed +-pi/2")
    {
        auto constexpr manyPixels = 9999999999;

        auto actions = momentaryMap(dMovement({0, manyPixels}));
        for(auto const& action : actions)
            controller.handleMomentaryAction(action);
        controller.update(player.state(), dontCare);
        REQUIRE(player.state().lookAtOffset.y < glm::pi<double>() / 2.0);

        actions = momentaryMap(dMovement({0, -manyPixels}));
        for(auto const& action : actions)
            controller.handleMomentaryAction(action);
        controller.update(player.state(), dontCare);
        REQUIRE(player.state().lookAtOffset.y > -glm::pi<double>() / 2);
    }
}

TEST_CASE("WalkController controlls player scale", "[WalkController]")
{
    using namespace Input;

    auto player     = Player();
    auto controller = WalkController{};
    auto dontCare   = 0.0;

    auto constexpr inKey   = Key::J;
    auto constexpr outKey  = Key::K;
    auto constexpr autoKey = Key::O;

    StateMap persistentMap;
    persistentMap.add({inKey}, State::ZoomingIn);
    persistentMap.add({outKey}, State::ZoomingOut);

    auto momentaryMap = MomentaryActionsMap();
    momentaryMap.add({autoKey}, Trigger::ToggleAutoZoom);

    REQUIRE(player.state().scale == 1.0);

    SECTION("Player can shrink")
    {
        persistentMap.updateState(KeyDown{inKey});
        controller.updateState(persistentMap);
        controller.update(player.state(), 1.0);
        auto scale = player.state().scale;
        REQUIRE(scale < 1.0);

        persistentMap.updateState(KeyUp{inKey});
        controller.updateState(persistentMap);
        controller.update(player.state(), 1.0);
        REQUIRE(player.state().scale == scale);
    }

    SECTION("Player can grow")
    {
        persistentMap.updateState(KeyDown{outKey});
        controller.updateState(persistentMap);
        controller.update(player.state(), 1.0);
        auto scale = player.state().scale;
        REQUIRE(scale > 1.0);

        persistentMap.updateState(KeyUp{outKey});
        controller.updateState(persistentMap);
        controller.update(player.state(), 1.0);
        REQUIRE(player.state().scale == scale);
    }

    SECTION("Growing and shrinking at the same time does nothing")
    {
        persistentMap.updateState(KeyDown{inKey});
        persistentMap.updateState(KeyDown{outKey});
        controller.updateState(persistentMap);
        controller.update(player.state(), 1.0);
        REQUIRE(player.state().scale == Approx(1.0));
    }

    SECTION("Player can enable automatic zooming")
    {
        auto height               = 0.200468;
        player.state().position.y = height;

        auto actions = momentaryMap(KeyDown{autoKey});
        for(auto const& action : actions) {
            controller.handleMomentaryAction(action);
        }

        controller.updateState(persistentMap);
        controller.update(player.state(), dontCare);
        REQUIRE(player.state().scale == height);
    }

    SECTION("Scaling happens exponentially")
    {
        auto dt = 3.60887;
        persistentMap.updateState(KeyDown{inKey});
        controller.updateState(persistentMap);

        auto playerReference = Player();
        controller.update(playerReference.state(), dt);
        auto scaleReference = playerReference.state().scale;

        SECTION("Double the delta time squares the scale change")
        {
            controller.update(player.state(), 2.0 * dt);
            REQUIRE(scaleReference * scaleReference
                    == Approx(player.state().scale));
        }

        SECTION("Twice updating squares the scale change")
        {
            controller.update(player.state(), dt);
            controller.update(player.state(), dt);
            REQUIRE(scaleReference * scaleReference
                    == Approx(player.state().scale));
        }
    }
}
