#ifndef MANDELLANDSCAPE_SETTINGS_H
#define MANDELLANDSCAPE_SETTINGS_H

#include <type_traits>

namespace Settings {
template<
        typename T,
        int _uid,
        typename = std::enable_if_t<std::is_default_constructible_v<T>>>
struct Setting {
    using Type               = T;
    static constexpr int uid = _uid;
};

template<typename S>
inline bool constexpr isSetting =
        std::is_same_v<S, Setting<typename S::Type, S::uid>>;

template<typename S>
using RequireSetting = std::enable_if_t<isSetting<S>>;

// Only create settings here. __LINE__ is to give each setting a unique id
using WindowHeight = Setting<int, __LINE__>;
using WindowWidth  = Setting<int, __LINE__>;
}    // namespace Settings

#endif    // MANDELLANDSCAPE_SETTINGS_H
