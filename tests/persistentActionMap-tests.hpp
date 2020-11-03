#ifndef MANDELLANDSCAPE_PERSISTENTACTIONMAP_TESTS_HPP
#define MANDELLANDSCAPE_PERSISTENTACTIONMAP_TESTS_HPP

#include <map>
#include <set>

#include "glfwEnums.hpp"
#include "persistentActionMap.hpp"
#include "event.hpp"
#include "util.hpp"

namespace PersistentActionMapTests {

TEST_CASE(
        "PersistentActionMap keeps track of keyboard state",
        "[PersistentActionMap]")
{
    using namespace Input;

    PersistentActionMap map;
    map.add(Key::U, PersistentAction::MoveForwards);
    SECTION("State is updated")
    {
        map.updateState(KeyDown{Key::U});
        REQUIRE(map(PersistentAction::MoveForwards));

        map.updateState(KeyUp{Key::U});
        REQUIRE(!map(PersistentAction::MoveForwards));
    }

    SECTION("Action defaults to off")
    {
        REQUIRE(!map(PersistentAction::MoveBackwards));
    }
}

}    // namespace PersistentActionMapTests

#endif
