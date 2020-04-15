#ifndef MANDELLANDSCAPE_SETTINGS_H
#define MANDELLANDSCAPE_SETTINGS_H

#include "mandelTypeTraits.h"

namespace Settings {
template<
        typename T,
        int _uid,
        typename = Require<std::is_move_constructible_v<T>>>
struct Setting {
    using Type               = T;
    static constexpr int uid = _uid;
};

template<typename T>
using RequireSetting = RequireSame<T, Setting<typename T::Type, T::uid>>;

// Only create settings here. __LINE__ is to give each setting a unique id
using WindowHeight      = Setting<int, __LINE__>;
using WindowWidth       = Setting<int, __LINE__>;
using ClippingPlaneNear = Setting<float, __LINE__>;
using ClippingPlaneFar  = Setting<float, __LINE__>;
using FOV               = Setting<float, __LINE__>;
using UseDeepShader     = Setting<bool, __LINE__>;
using AutoZoom          = Setting<int, __LINE__>;
}    // namespace Settings

#endif    // MANDELLANDSCAPE_SETTINGS_H
