#ifndef MANDELLANDSCAPE_UTILS_TESTS_H
#define MANDELLANDSCAPE_UTILS_TESTS_H

#include <queue>
#include <optional>

#include <catch2/catch.hpp>

#include <utils.h>

TEST_CASE(
        "util::pop pops and returns the top element of a std::queu",
        "[utils]")
{
    auto queue = std::queue<int>();

    SECTION("Empty queue should return a std::optional<std::nullopt>")
    {
        auto nullopt = util::pop(queue);
        REQUIRE(nullopt.has_value());
    }
}

#endif    // MANDELLANDSCAPE_UTILS_TESTS_H
