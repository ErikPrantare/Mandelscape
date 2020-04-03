#ifndef MANDELLANDSCAPE_SETTINGS_H
#define MANDELLANDSCAPE_SETTINGS_H

#include <type_traits>

namespace Settings {
class Secret {
    struct Token {};

    template<typename, int, typename>
    friend struct Setting;

    friend class Config;

    template<
            typename T,
            typename = typename std::enable_if_t<std::is_same_v<
                    typename T::Token,
                    typename Settings::Secret::Token>>>
    struct enable_if_setting {
        using type = T;
    };

    template<typename T>
    using enable_if_setting_t = typename enable_if_setting<T>::type;
};

template<
        typename T,
        int _uid,
        typename =
                typename std::enable_if_t<std::is_default_constructible_v<T>>>
struct Setting {
public:
    using type               = T;
    static constexpr int uid = _uid;
    using Token              = Secret::Token;
};

// Only create settings here. __LINE__ is to give each setting a unique id
using WindowHeight = Setting<int, __LINE__>;
using WindowWidth  = Setting<int, __LINE__>;
}    // namespace Settings

#endif    // MANDELLANDSCAPE_SETTINGS_H
