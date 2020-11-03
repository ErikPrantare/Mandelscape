#include "persistentActionMap.hpp"

#include "util.hpp"

auto
PersistentActionMap::add(Input::Key key, PersistentAction action) -> void
{
    m_keyMap[key].insert(action);
}
auto
PersistentActionMap::updateState(Event const& event) -> void
{
    auto const setValue = [this](Input::Key key, bool value) {
        auto const actions = m_keyMap[key];
        for(auto const& action : actions) {
            m_actionMap[action] = value;
        }
    };

    std::visit(
            util::Overload{
                    [&setValue](KeyDown key) { setValue(key.code, true); },
                    [&setValue](KeyUp key) { setValue(key.code, false); },

                    // default
                    util::unaryNOP},
            event);
}

auto
PersistentActionMap::operator()(PersistentAction action) const -> bool
{
    return util::contains(m_actionMap, action) && m_actionMap.at(action);
}
