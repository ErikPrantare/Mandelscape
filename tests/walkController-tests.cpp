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

auto const sameMutation = [](auto const& f, auto const& g) -> bool {
    auto mutated1 = Player();
    auto mutated2 = Player();
    f(mutated1);
    g(mutated2);
    return mutated1 == Approximate{mutated2};
};

auto const bindApplyEvents = [](MomentaryActionsMap const& momentaryMap,
                                StateMap& stateMap,
                                WalkController& controller) {
    auto const applyEvents = [&momentaryMap, &stateMap, &controller](
                                     std::vector<Event> const& events,
                                     Player& player) {
        auto constexpr dontCare = 13.37;

        for(auto const& event : events) {
            stateMap.updateState(event);
            controller.updateState(stateMap);
            controller.update(player, dontCare);

            auto actions = momentaryMap(event);
            for(auto const& action : actions) {
                controller.handleMomentaryAction(action);
            }
            controller.update(player, dontCare);
        }
    };

    // Curry for ease of use with sameMutation
    return [applyEvents](std::vector<Event> const& events) {
        return [applyEvents, events](Player& player) {
            applyEvents(events, player);
        };
    };
};

TEST_CASE("WalkController handles movement keys", "[WalkController]")
{
    using namespace Input;
    auto constexpr frontKey  = Key::W;
    auto constexpr backKey   = Key::S;
    auto constexpr leftKey   = Key::A;
    auto constexpr rightKey  = Key::D;
    auto constexpr zoomInKey = Key::Z;
    auto constexpr runKey    = Key::LeftShift;

    auto controller = WalkController{};

    auto stateMap = StateMap();
    stateMap.add({frontKey}, State::MovingForwards);
    stateMap.add({backKey}, State::MovingBackwards);
    stateMap.add({leftKey}, State::MovingLeft);
    stateMap.add({rightKey}, State::MovingRight);
    stateMap.add({zoomInKey}, State::ZoomingIn);
    stateMap.add({frontKey, (int)Mod::Shift}, State::RunningForwards);
    stateMap.add({backKey, (int)Mod::Shift}, State::RunningBackwards);
    stateMap.add({leftKey, (int)Mod::Shift}, State::RunningLeft);
    stateMap.add({rightKey, (int)Mod::Shift}, State::RunningRight);

    auto momentaryMap = MomentaryActionsMap();

    auto player = Player();

    REQUIRE(player.truePosition() == zero);

    SECTION("No buttons held -> no movement")
    {
        controller.update(player, 1.0);
        REQUIRE(player.truePosition() == zero);
    }

    SECTION("Player walks in base directions correctly")
    {
        auto testDirection = [&controller,
                              &stateMap](Key key, glm::dvec3 direction) {
            auto player = Player();

            stateMap.updateState(KeyDown{key});
            controller.updateState(stateMap);
            controller.update(player, 1.0);
            REQUIRE(player.truePosition() == direction);

            controller.updateState(stateMap);
            controller.update(player, 1.0);
            REQUIRE(player.truePosition() == 2.0 * direction);

            stateMap.updateState(KeyUp{key});
            controller.updateState(stateMap);
            controller.update(player, 1.0);
            REQUIRE(player.truePosition() == 2.0 * direction);
        };

        testDirection(frontKey, front);
        testDirection(leftKey, left);
        testDirection(backKey, back);
        testDirection(rightKey, right);
    }

    SECTION("Combination of inputs accelerate in multiple directions")
    {
        stateMap.updateState(KeyDown{backKey});
        stateMap.updateState(KeyDown{leftKey});
        controller.updateState(stateMap);
        controller.update(player, 1.0);
        REQUIRE(player.truePosition() == back + left);
    }

    SECTION("Movement in opposite directions yields no velocity")
    {
        stateMap.updateState(KeyDown{leftKey});
        stateMap.updateState(KeyDown{rightKey});
        controller.updateState(stateMap);
        controller.update(player, 1.0);
        REQUIRE(player.truePosition() == zero);

        stateMap.updateState(KeyDown{backKey});
        stateMap.updateState(KeyDown{frontKey});
        controller.updateState(stateMap);
        controller.update(player, 1.0);
        REQUIRE(player.truePosition() == zero);
    }

    SECTION("Movement is dependent on player scale")
    {
        auto const referencePlayer = player;

        stateMap.updateState(KeyDown{leftKey});
        controller.updateState(stateMap);
        stateMap.updateState(KeyUp{leftKey});

        controller.update(player, 1.0);
        auto const dUnscaledPos =
                player.truePosition() - referencePlayer.truePosition();

        player = referencePlayer;

        stateMap.updateState(KeyDown{zoomInKey});
        controller.updateState(stateMap);
        stateMap.updateState(KeyUp{zoomInKey});
        controller.update(player, 1.0);

        stateMap.updateState(KeyDown{leftKey});
        controller.updateState(stateMap);
        stateMap.updateState(KeyUp{leftKey});
        controller.update(player, 1.0);

        auto const scaleFactor = player.scale();
        auto const dScaledPos =
                player.truePosition() - referencePlayer.truePosition();

        REQUIRE(scaleFactor * dUnscaledPos == Approximate{dScaledPos});
    }

    auto applyEvents = bindApplyEvents(momentaryMap, stateMap, controller);

    SECTION("Movement is dependent on player look direction")
    {
        // Walking along both sides of rhombus ends up at the same place
        REQUIRE(sameMutation(
                applyEvents({
                        KeyDown{frontKey},
                        KeyUp{frontKey},
                        MouseMove{{}, {42, 0}},
                        KeyDown{frontKey},
                        KeyUp{frontKey},
                        MouseMove{{}, {-42, 0}},
                }),
                applyEvents({
                        MouseMove{{}, {42, 0}},
                        KeyDown{frontKey},
                        KeyUp{frontKey},
                        MouseMove{{}, {-42, 0}},
                        KeyDown{frontKey},
                        KeyUp{frontKey},
                })));

        // ... but not if we don't walk all the way
        REQUIRE(!sameMutation(
                applyEvents({
                        KeyDown{frontKey},
                        KeyUp{frontKey},
                        MouseMove{{}, {42, 0}},
                        MouseMove{{}, {-42, 0}},
                }),
                applyEvents({
                        MouseMove{{}, {42, 0}},
                        KeyDown{frontKey},
                        KeyUp{frontKey},
                        MouseMove{{}, {-42, 0}},
                })));
    }

    SECTION("Running doubles player speed")
    {
        REQUIRE(!sameMutation(
                applyEvents({
                        KeyDown{runKey},
                        KeyDown{frontKey},
                }),
                applyEvents({
                        KeyDown{frontKey},
                        KeyUp{frontKey},
                        KeyDown{frontKey},
                        KeyUp{frontKey},
                })));
    }
}

TEST_CASE("WalkController handles mouse movement", "[WalkController]")
{
    auto controller         = WalkController{};
    auto player             = Player();
    auto const momentaryMap = MomentaryActionsMap();
    auto stateMap           = StateMap();

    auto constexpr dontCare = 0.0;

    auto dMovement = [](glm::dvec2 dPos) {
        // CPP20: {.x = ...}
        return MouseMove{{0.0, 0.0}, dPos};
    };

    REQUIRE(player.truePosition() == zero);

    SECTION("Mouse movement")
    {
        auto directionTest = [&](glm::dvec2 pixelsMoved) {
            auto const origPlayer = player;

            auto actions = momentaryMap(dMovement(pixelsMoved));
            for(auto const& action : actions) {
                controller.handleMomentaryAction(action);
            }
            controller.update(player, dontCare);
            auto const newDirectionPlayer = player;
            REQUIRE(newDirectionPlayer != Approximate{origPlayer});

            actions = momentaryMap(dMovement(-pixelsMoved));
            for(auto const& action : actions) {
                controller.handleMomentaryAction(action);
            }
            controller.update(player, dontCare);
            REQUIRE(player == Approximate{origPlayer});

            actions = momentaryMap(dMovement(-pixelsMoved));
            for(auto const& action : actions) {
                controller.handleMomentaryAction(action);
            }
            controller.update(player, dontCare);
            REQUIRE(player != Approximate{origPlayer});
            REQUIRE(player != Approximate{newDirectionPlayer});
        };

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
    }

    auto const applyEvents =
            bindApplyEvents(momentaryMap, stateMap, controller);

    SECTION("Multiple mouse events before update are compounded")
    {
        auto constexpr pixelsMoved = 73;

        REQUIRE(sameMutation(
                [&momentaryMap, &controller, &dMovement](Player& player) {
                    auto actions = momentaryMap(dMovement({pixelsMoved, 0}));
                    for(auto const& action : actions) {
                        controller.handleMomentaryAction(action);
                        controller.handleMomentaryAction(action);
                    }
                    controller.update(player, dontCare);
                },
                applyEvents({dMovement({2 * pixelsMoved, 0})})));
    }

    SECTION("Vertical look direction caps")
    {
        auto constexpr manyPixels = 9999999;

        REQUIRE(sameMutation(
                applyEvents({dMovement({0, manyPixels})}),
                applyEvents({dMovement({0, 10 * manyPixels})})));

        REQUIRE(sameMutation(
                applyEvents({dMovement({0, -manyPixels})}),
                applyEvents({dMovement({0, -10 * manyPixels})})));
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

    REQUIRE(player.scale() == 1.0);

    SECTION("Player can shrink")
    {
        persistentMap.updateState(KeyDown{inKey});
        controller.updateState(persistentMap);
        controller.update(player, 1.0);
        auto scale = player.scale();
        REQUIRE(scale < 1.0);

        persistentMap.updateState(KeyUp{inKey});
        controller.updateState(persistentMap);
        controller.update(player, 1.0);
        REQUIRE(player.scale() == scale);
    }

    SECTION("Player can grow")
    {
        persistentMap.updateState(KeyDown{outKey});
        controller.updateState(persistentMap);
        controller.update(player, 1.0);
        auto scale = player.scale();
        REQUIRE(scale > 1.0);

        persistentMap.updateState(KeyUp{outKey});
        controller.updateState(persistentMap);
        controller.update(player, 1.0);
        REQUIRE(player.scale() == scale);
    }

    SECTION("Growing and shrinking at the same time does nothing")
    {
        persistentMap.updateState(KeyDown{inKey});
        persistentMap.updateState(KeyDown{outKey});
        controller.updateState(persistentMap);
        controller.update(player, 1.0);
        REQUIRE(player.scale() == Approx(1.0));
    }

    SECTION("Player can enable automatic zooming")
    {
        auto height = 0.200468;
        player.updateFeetAltitude(height);

        auto actions = momentaryMap(KeyDown{autoKey});
        for(auto const& action : actions) {
            controller.handleMomentaryAction(action);
        }

        controller.updateState(persistentMap);
        controller.update(player, dontCare);
        REQUIRE(player.scale() == height);
    }

    SECTION("Scaling happens exponentially")
    {
        auto dt = 3.60887;
        persistentMap.updateState(KeyDown{inKey});
        controller.updateState(persistentMap);

        auto playerReference = Player();
        controller.update(playerReference, dt);
        auto scaleReference = playerReference.scale();

        SECTION("Double the delta time squares the scale change")
        {
            controller.update(player, 2.0 * dt);
            REQUIRE(scaleReference * scaleReference == Approx(player.scale()));
        }

        SECTION("Twice updating squares the scale change")
        {
            controller.update(player, dt);
            controller.update(player, dt);
            REQUIRE(scaleReference * scaleReference == Approx(player.scale()));
        }
    }
}
