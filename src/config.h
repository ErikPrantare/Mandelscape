#ifndef MANDELLANDSCAPE_CONFIG_H
#define MANDELLANDSCAPE_CONFIG_H

#include <map>
#include <vector>
#include <any>
#include <functional>

#include "mandelTypeTraits.h"
#include "settings.h"
#include "utils.h"

namespace Settings {

class Config {
public:
    Config() = default;

    template<typename Setting, typename = RequireSetting<Setting>>
    void
    set(typename Setting::Type&& newValue)
    {
        using SettingType = typename Setting::Type;

        if(util::contains(m_settings, Setting::uid))
            m_settings[Setting::uid] =
                    std::any(std::forward<SettingType>(newValue));
        else
            m_settings.emplace(
                    Setting::uid,
                    std::forward<SettingType>(newValue));

        for(auto const& callback : m_callbacks[Setting::uid]) {
            callback(m_settings[Setting::uid]);
        }
    }

    template<typename Setting, typename = RequireSetting<Setting>>
    typename Setting::Type
    get() const
    {
        if(util::contains(m_settings, Setting::uid))
            return std::any_cast<typename Setting::Type>(
                    m_settings.at(Setting::uid));
        else
            throw std::runtime_error(
                    "Setting " + std::to_string(Setting::uid)
                    + " has not been set.");
    }

    template<
            typename Setting,
            typename Callable,
            typename = RequireSetting<Setting>,
            typename = RequireCallableWith<Callable, typename Setting::Type>>
    void
    onStateChange(Callable&& callback)
    {
        m_callbacks[Setting::uid].emplace_back(
                [callback](std::any const& value) {
                    callback(std::any_cast<typename Setting::Type>(value));
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
