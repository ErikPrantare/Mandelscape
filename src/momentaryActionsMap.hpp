#ifndef MANDELLANDSCAPE_TRIGGERACTIONSMAP_HPP
#define MANDELLANDSCAPE_TRIGGERACTIONSMAP_HPP

#include <map>
#include <set>
#include <variant>

#include "event.hpp"
#include "momentaryAction.hpp"
#include "glfwEnums.hpp"

class MomentaryActionsMap {
public:
    auto
    add(Input::Key key, TriggerAction action) -> void;

    auto
    operator()(Event const& event) const -> std::set<MomentaryAction>;

private:
    auto
    getActions(KeyDown key) const -> std::set<MomentaryAction>;

    auto
    getActions(MouseMove mouse) const -> std::set<MomentaryAction>;

    auto
    getActions(KeyUp key) const -> std::set<MomentaryAction>;

    auto
    getActions(MouseButtonUp mouse) const -> std::set<MomentaryAction>;

    auto
    getActions(MouseButtonDown mouse) const -> std::set<MomentaryAction>;

    std::map<Input::Key, std::set<MomentaryAction>> m_keyMap;
};

#endif
