#include "persistentActionMap.hpp"

#include "util.hpp"

auto
PersistentActionMap::add(Input::Key key, PersistentAction action) -> void
{
    m_keyMap[key].insert(action);
}

auto
PersistentActionMap::add(Input::MouseButton button, PersistentAction action)
        -> void
{
    m_mouseButtonMap[button].insert(action);
}

auto
PersistentActionMap::updateState(Event const& event) -> void
{
    auto const setActions = [this](std::set<PersistentAction>& actions,
                                   bool value) {
        for(auto const& action : actions) {
            m_actionMap[action] = value;
        }
    };

    std::visit(
            util::Overload{
                    [this, &setActions](KeyDown key) {
                        setActions(m_keyMap[key.code], true);
                    },
                    [this, &setActions](KeyUp key) {
                        setActions(m_keyMap[key.code], false);
                    },

                    [this, &setActions](MouseButtonDown button) {
                        setActions(m_mouseButtonMap[button.button], true);
                    },
                    [this, &setActions](MouseButtonUp button) {
                        setActions(m_mouseButtonMap[button.button], false);
                    },
                    // default
                    util::unaryNOP},
            event);
}

auto
PersistentActionMap::operator()(PersistentAction action) const -> bool
{
    return util::contains(m_actionMap, action) && m_actionMap.at(action);
}
