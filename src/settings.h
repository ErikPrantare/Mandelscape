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

// Only create Setting s in here
#define GenSetting(Name, Type) using Name = Setting<Type, __LINE__>;

}    // namespace Settings

#endif    // SETTINGS_H
