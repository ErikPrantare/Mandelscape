#ifndef MANDELLANDSCAPE_META_CONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_META_CONTROLLER_TESTS_HPP

#include "metaController.hpp"

#include <catch2/catch.hpp>

namespace MetaControllerTests {

TEST_CASE(
        "MetaController helper functions",
        "[isNextController][safeIncrement]")
{
    SECTION("isNextController returns true for a 'c' keypress")
    {
        auto constexpr event = Event{KeyDown{GLFW_KEY_C}};
        STATIC_REQUIRE(isNextController(event));
    }
    SECTION("isNextController can accept an Event not holding a KeyDown")
    {
        auto constexpr event = Event{MouseMove{}};
        STATIC_REQUIRE_FALSE(isNextController(event));
    }

    auto constexpr start = 0;
    static_assert(start == 0);

    auto constexpr bound = 10;
    static_assert(bound > 1);

    auto constexpr incrementLoop =
            [](auto const start, auto const bound, auto const times) {
                auto result = start;
                for(auto i = 0; i < times; ++i) {
                    result = safeIncrement(result, bound);
                }

                return result;
            };

    SECTION("safeIncrement increments by 1")
    {
        auto constexpr increments = 5;
        auto constexpr result     = incrementLoop(start, bound, increments);

        STATIC_REQUIRE(result == start + increments);
    }

    SECTION("safeIncrement wraps around to 0 if it increments to bound")
    {
        auto constexpr increments = bound + 1;
        auto constexpr result     = incrementLoop(start, bound, increments);

        STATIC_REQUIRE(result == 1);
    }
}

TEST_CASE("MetaController construction", "[MetaController]")
{}

}    // namespace MetaControllerTests

#endif    // MANDELLANDSCAPE_META_CONTROLLER_TESTS_HPP
