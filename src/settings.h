#ifndef MANDELLANDSCAPE_SETTINGS_H
#define MANDELLANDSCAPE_SETTINGS_H

#include <type_traits>

class Config;

namespace Settings {
struct Secret {
private:
    friend Config;
    using Token = int;
    template<typename, int, typename>
    friend struct Setting;
};

template<
        typename T,
        int _uid,
        typename =
                typename std::enable_if_t<std::is_default_constructible_v<T>>>
struct Setting {
public:
    using Type               = T;
    static constexpr int uid = _uid;
    using Token              = Secret::Token;
};

// Only create settings here. __LINE__ is to give each setting a unique id
using WindowHeight = Setting<int, __LINE__>;
using WindowWidth  = Setting<int, __LINE__>;

}    // namespace Settings

#endif    // SETTINGS_H
