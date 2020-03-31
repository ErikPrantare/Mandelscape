#ifndef CONFIG_H
#define CONFIG_H

#include <variant>
#include <string>
#include <vector>

enum class Setting { NAME, STATE1, STATE2, STATE3, STATE4_but_forgot_type };

namespace {
template<Setting setting>
struct SettingType {};

#define GenSettingType(SettingName, Type)          \
    namespace {                                    \
        template<>                                 \
        struct SettingType<Setting::SettingName> { \
            using type = Type;                     \
        };                                         \
    }
}    // namespace

GenSettingType(NAME, std::string);
GenSettingType(STATE1, int);
GenSettingType(STATE2, double);
GenSettingType(STATE3, std::vector<int>);

class Config {
public:
    Config() = default;

    template<
            Setting setting,
            typename T = typename SettingType<setting>::type,
            typename   = typename std::enable_if_t<
                    std::is_same_v<typename SettingType<setting>::type, T>>>
    void
    change_setting(T const& newValue)
    {}

private:
};

#endif    // CONFIG_H
