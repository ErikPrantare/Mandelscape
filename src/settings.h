#ifndef SETTINGS_H
#define SETTINGS_H

#include <type_traits>

namespace Settings {

template<
        typename Type,
        int UID,
        typename = typename std::enable_if_t<
                std::is_default_constructible_v<Type>>>
struct Setting {
    using type = Type;
    enum { uid = UID };
    static bool constexpr isSetting = true;
};

// Only create Setting s in here
#define GenSetting(Name, Type) using Name = Setting<Type, __LINE__>;

}    // namespace Settings

#endif    // SETTINGS_H
