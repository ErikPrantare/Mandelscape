#ifndef MANDELLANDSCAPE_MOMENTARYACTION_HPP
#define MANDELLANDSCAPE_MOMENTARYACTION_HPP

#include <variant>

enum class TriggerAction { IncreaseIterations, DecreaseIterations };
struct MouseDelta {
    double dx;
    double dy;
};
bool
operator<(MouseDelta a, MouseDelta b);

using MomentaryAction = std::variant<TriggerAction, MouseDelta>;

#endif    // MANDELLANDSCAPE_MOMENTARYACTION_HPP
