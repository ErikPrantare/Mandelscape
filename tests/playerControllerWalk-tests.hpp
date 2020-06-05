#ifndef MANDELLANDSCAPE_PLAYERCONTROLLERWALK_TESTS_HPP
#define MANDELLANDSCAPE_PLAYERCONTROLLERWALK_TESTS_HPP

#include <memory>

#include <GLFW/glfw3.h>
#include "event.hpp"

#include "playerControllerWalk.hpp"

namespace {

glm::dvec3 constexpr left{-1.0, 0.0, 0.0};
glm::dvec3 constexpr right{1.0, 0.0, 0.0};
glm::dvec3 constexpr front{0.0, 0.0, -1.0};
glm::dvec3 constexpr back{0.0, 0.0, 1.0};

glm::dvec3 constexpr zero{0.0, 0.0, 0.0};

TEST_CASE(
        "PlayerControllerWalk handles movement keys",
        "[PlayerControllerWalk]")
{
    auto controller =
            std::unique_ptr<PlayerController>(new PlayerControllerWalk);

    REQUIRE(controller->velocity() == zero);

    SECTION("\"A\" button hold moves left")
    {
        controller->handleEvent(KeyDown{GLFW_KEY_A});
        REQUIRE(controller->velocity() == left);
        controller->handleEvent(KeyUp{GLFW_KEY_A});
        REQUIRE(controller->velocity() == zero);
    }
    SECTION("\"D\" button hold moves right")
    {
        controller->handleEvent(KeyDown{GLFW_KEY_D});
        REQUIRE(controller->velocity() == right);
        controller->handleEvent(KeyUp{GLFW_KEY_D});
        REQUIRE(controller->velocity() == zero);
    }
    SECTION("\"W\" button hold moves forwards")
    {
        controller->handleEvent(KeyDown{GLFW_KEY_W});
        REQUIRE(controller->velocity() == front);
        controller->handleEvent(KeyUp{GLFW_KEY_W});
        REQUIRE(controller->velocity() == zero);
    }
    SECTION("\"S\" button hold moves backwards")
    {
        controller->handleEvent(KeyDown{GLFW_KEY_S});
        REQUIRE(controller->velocity() == back);
        controller->handleEvent(KeyUp{GLFW_KEY_S});
        REQUIRE(controller->velocity() == zero);
    }

    SECTION("Combination of inputs accelerate in multiple directions")
    {
        controller->handleEvent(KeyDown{GLFW_KEY_S});
        controller->handleEvent(KeyDown{GLFW_KEY_A});
        REQUIRE(controller->velocity() == back + left);
    }

    SECTION("Movement in opposite directions yields no velocity")
    {
        controller->handleEvent(KeyDown{GLFW_KEY_A});
        controller->handleEvent(KeyDown{GLFW_KEY_D});
        REQUIRE(controller->velocity() == zero);
        controller->handleEvent(KeyDown{GLFW_KEY_W});
        controller->handleEvent(KeyDown{GLFW_KEY_S});
        REQUIRE(controller->velocity() == zero);
    }
}

}    // namespace

#endif
