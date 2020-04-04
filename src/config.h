#ifndef MANDELLANDSCAPE_CONFIG_H
#define MANDELLANDSCAPE_CONFIG_H

#include <type_traits>
#include <map>
#include <vector>
#include <any>
#include <functional>

#include "settings.h"

namespace Settings {

class Config {
    template<typename Callable, typename Arg>
    static bool constexpr returns =
            std::is_same_v<Arg, std::invoke_result_t<Callable, Arg>>;

    template<typename Callable, typename SettingType>
    using RequireEndomorphismOf =
            std::enable_if_t<returns<Callable, SettingType>>;

    template<typename Callable, typename Arg>
    using RequireCallableWith = std::invoke_result_t<Callable, Arg>;

public:
    Config() = default;

    template<typename Setting, typename = RequireSetting<Setting>>
    void
    set(typename Setting::Type newValue)
    {
        std::any const value     = std::any(newValue);
        m_settings[Setting::uid] = value;

        for(auto const& callback : m_callbacks[Setting::uid]) {
            callback(value);
        }
    }

    template<typename Setting, typename = RequireSetting<Setting>>
    typename Setting::Type
    get()
    {
        return std::any_cast<typename Setting::Type>(m_settings[Setting::uid]);
    }

    template<
            typename Setting,
            typename Callable,
            typename = RequireSetting<Setting>,
            typename = RequireCallableWith<Callable, typename Setting::Type>>
    void
    onStateChange(Callable const callback)
    {
        m_callbacks[Setting::uid].push_back([callback](std::any const& value) {
            auto a = std::any_cast<typename Setting::Type>(value);
            callback(a);
        });
    }

    template<
            typename Setting,
            typename Callable,
            typename = RequireSetting<Setting>,
            typename = RequireEndomorphismOf<Callable, typename Setting::Type>>
    void
    on(Callable const& callable)
    {
        set<Setting>(callable(get<Setting>()));
    }

private:
    using FunctionSig = std::function<void(std::any const&)>;

    std::map<int, std::any> m_settings;
    std::map<int, std::vector<FunctionSig>> m_callbacks;
};

}    // namespace Settings

#endif    // MANDELLANDSCAPE_CONFIG_H
