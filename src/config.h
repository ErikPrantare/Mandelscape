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
    using FunctionSig = std::function<void(std::any const&)>;

public:
    Config() = default;

    template<typename Setting, typename = Secret::enable_if_setting_t<Setting>>
    void
    set(typename Setting::type newValue)
    {
        std::any const value     = std::any(newValue);
        m_settings[Setting::uid] = value;

        for(auto const& callback : m_callbacks[Setting::uid]) {
            callback(value);
        }
    }

    template<typename Setting, typename = Secret::enable_if_setting_t<Setting>>
    typename Setting::type
    get()
    {
        return std::any_cast<typename Setting::type>(m_settings[Setting::uid]);
    }

    template<
            typename Setting,
            typename Function,
            typename = Secret::enable_if_setting_t<Setting>>
    void
    subscribe(Function const& callback)
    {
        m_callbacks[Setting::uid].push_back(
                std::function([callback](std::any v) {
                    callback(std::any_cast<Setting::type>(v));
                }));
    }

    template<
            typename Setting,
            typename Function,
            typename = Secret::enable_if_setting_t<Setting>>
    void
    on(Function const& f)
    {
        set<Setting>(f(get<Setting>()));
    }

private:
    std::map<int, std::any> m_settings;
    std::map<int, std::vector<FunctionSig>> m_callbacks;
};

}    // namespace Settings

#endif    // MANDELLANDSCAPE_CONFIG_H
