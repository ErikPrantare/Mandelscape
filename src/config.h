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
    template<typename Callable, typename T>
    static bool constexpr returns_argument_type_v =
            std::is_same_v<T, std::invoke_result_t<Callable, T>>;

    template<typename Callable, typename SettingType>
    using RequireCallable =
            std::enable_if_t<returns_argument_type_v<Callable, SettingType>>;

public:
    Config() = default;

    template<typename Setting, typename = Secret::RequireSetting<Setting>>
    void
    set(typename Setting::type newValue)
    {
        std::any const value     = std::any(newValue);
        m_settings[Setting::uid] = value;

        for(auto const& callback : m_callbacks[Setting::uid]) {
            callback(value);
        }
    }

    template<typename Setting, typename = Secret::RequireSetting<Setting>>
    typename Setting::type
    get()
    {
        return std::any_cast<typename Setting::type>(m_settings[Setting::uid]);
    }

    template<
            typename Setting,
            typename Callable,
            typename = Secret::RequireSetting<Setting>,
            typename = RequireCallable<Callable, typename Setting::type>>
    void
    subscribe(Callable const callback)
    {
        m_callbacks[Setting::uid].push_back([callback](std::any const& value) {
            callback(std::any_cast<typename Setting::type>(value));
        });
    }

    template<
            typename Setting,
            typename Callable,
            typename = Secret::RequireSetting<Setting>,
            typename = RequireCallable<Callable, typename Setting::type>>
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
