#include "momentaryAction.hpp"

#include <tuple>

auto
operator<(MouseDelta a, MouseDelta b) -> bool
{
    return std::tie(a.dx, a.dy) < std::tie(b.dx, b.dy);
}

auto
sameAction(MomentaryAction const& action, TriggerAction triggerAction) -> bool
{
    return std::holds_alternative<TriggerAction>(action)
           && std::get<TriggerAction>(action) == triggerAction;
}
