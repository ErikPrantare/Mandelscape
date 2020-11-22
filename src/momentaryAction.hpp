#ifndef MANDELLANDSCAPE_MOMENTARYACTION_HPP
#define MANDELLANDSCAPE_MOMENTARYACTION_HPP

#include <variant>

enum class TriggerAction {
    IncreaseIterations,
    DecreaseIterations,
    ToggleAutoWalk,
    ToggleAutoZoom,
    SwitchShader,
    TakeScreenshot,
    TogglePause,
    CloseWindow,
    ToggleFastMode,
};

struct MouseDelta {
    double dx;
    double dy;
};

using MomentaryAction = std::variant<TriggerAction, MouseDelta>;

auto
operator<(MouseDelta a, MouseDelta b) -> bool;

auto
sameAction(MomentaryAction const& action, TriggerAction triggerAction) -> bool;

#endif    // MANDELLANDSCAPE_MOMENTARYACTION_HPP
