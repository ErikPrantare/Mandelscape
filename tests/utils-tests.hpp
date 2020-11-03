#ifndef MANDELLANDSCAPE_UTILS_TESTS_HPP
#define MANDELLANDSCAPE_UTILS_TESTS_HPP

#include <queue>
#include <optional>

#include <catch2/catch.hpp>

#include "util.hpp"

namespace UtilsTests {

TEST_CASE(
        "util::pop pops and returns the top element of a std::queue",
        "[utils]")
{
    auto queue = std::queue<int>();

    SECTION("Empty queue should return a std::nullopt")
    {
        auto nullopt = util::pop(queue);
        REQUIRE(!nullopt.has_value());
    }
}

}    // namespace UtilsTests

#endif    // MANDELLANDSCAPE_UTILS_TESTS_HPP
