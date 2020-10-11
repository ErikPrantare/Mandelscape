#ifndef MANDELLANDSCAPE_TOACTIONS_TESTS_HPP
#define MANDELLANDSCAPE_TOACTIONS_TESTS_HPP

#include <map>
#include <set>

#include "glfwEnums.hpp"
#include "activeAction.hpp"

namespace ToActionTests {

TEST_CASE("ToActions maps from glfwEvent", "[ToActions]")
{
    using namespace Input;

    std::map<Key, std::set<ActiveAction>> toActions;
    toActions[Key::U] = {
            ActiveAction::IncreaseIterations,
            ActiveAction::DecreaseIterations};

    SECTION("Mapping gives correct result")
    {
        auto actions = toActions[Key::U];
        REQUIRE(actions.size() == 2);
        // CPP20 https://en.cppreference.com/w/cpp/container/set/contains
        REQUIRE(actions.find(ActiveAction::IncreaseIterations)
                != actions.end());
        REQUIRE(actions.find(ActiveAction::DecreaseIterations)
                != actions.end());
    }

    SECTION("No given mapping -> empty container")
    {
        auto actions = toActions[Key::K];
        REQUIRE(actions.size() == 0);
    }
}

}    // namespace ToActionTests

#endif
