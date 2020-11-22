#ifndef MANDELLANDSCAPE_PERSISTENTACTIONMAP_HPP
#define MANDELLANDSCAPE_PERSISTENTACTIONMAP_HPP

#include <map>
#include <set>

#include "glfwEnums.hpp"
#include "event.hpp"
#include "persistentAction.hpp"

class PersistentActionMap {
public:
    auto
    add(Input::Key key, PersistentAction action) -> void;

    auto
    add(Input::MouseButton button, PersistentAction action) -> void;

    auto
    updateState(Event const& event) -> void;

    auto
    operator()(PersistentAction action) const -> bool;

private:
    std::map<PersistentAction, bool> m_actionMap;
    std::map<Input::Key, std::set<PersistentAction>> m_keyMap;
    std::map<Input::MouseButton, std::set<PersistentAction>> m_mouseButtonMap;
};

#endif
