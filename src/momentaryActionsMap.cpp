#include "momentaryActionsMap.hpp"

#include "utils.hpp"

auto
MomentaryActionsMap::add(Input::Key key, TriggerAction action) -> void
{
    m_keyMap[key].insert({action});
}

auto
MomentaryActionsMap::operator()(Event const& event) const
        -> std::set<MomentaryAction>
{
    return std::visit([this](auto x) { return getActions(x); }, event);
}

auto
MomentaryActionsMap::getActions(KeyDown key) const -> std::set<MomentaryAction>
{
    if(util::contains(m_keyMap, (Input::Key)key.code)) {
        return m_keyMap.at((Input::Key)key.code);
    }
    return {};
}

auto
MomentaryActionsMap::getActions(MouseMove mouse) const
        -> std::set<MomentaryAction>
{
    return {MouseDelta{mouse.dx, mouse.dy}};
}

auto MomentaryActionsMap::getActions(KeyUp /*key*/) const
        -> std::set<MomentaryAction>
{
    return {};
}

auto MomentaryActionsMap::getActions(MouseButtonUp /*mouse*/) const
        -> std::set<MomentaryAction>
{
    return {};
}

auto MomentaryActionsMap::getActions(MouseButtonDown /*mouse*/) const
        -> std::set<MomentaryAction>
{
    return {};
}
