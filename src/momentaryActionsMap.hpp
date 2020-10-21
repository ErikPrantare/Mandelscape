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

    static auto
    getActions(MouseMove mouse) -> std::set<MomentaryAction>;

    static auto
    getActions(KeyUp key) -> std::set<MomentaryAction>;

    static auto
    getActions(MouseButtonUp mouse) -> std::set<MomentaryAction>;

    static auto
    getActions(MouseButtonDown mouse) -> std::set<MomentaryAction>;

    std::map<Input::Key, std::set<MomentaryAction>> m_keyMap;
};

#endif
