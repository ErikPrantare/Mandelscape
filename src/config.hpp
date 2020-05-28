#ifndef MANDELLANDSCAPE_CONFIG_H
#define MANDELLANDSCAPE_CONFIG_H

#include <map>
#include <vector>
#include <any>
#include <functional>
#include <iostream>

#include "mandelTypeTraits.hpp"
#include "settings.hpp"
#include "utils.hpp"

class Config {
private:
    template<typename Setting>
    using RequireSetting = Settings::RequireSetting<Setting>;

public:
    template<typename Setting, typename = RequireSetting<Setting>>
    void
    set(typename Setting::Type&& newValue)
    {
        using SettingType = typename Setting::Type;

        m_settings[Setting::uid] =
                std::any(std::forward<SettingType>(newValue));
    }

    template<typename Setting, typename = RequireSetting<Setting>>
    typename Setting::Type
    get() const
    {
        if(!util::contains(m_settings, Setting::uid)) {
            throw std::runtime_error(
                    "Setting " + std::to_string(Setting::uid)
                    + " has not been set.");
        }

        return std::any_cast<typename Setting::Type>(
                m_settings.at(Setting::uid));
    }

private:
    std::map<int, std::any> m_settings;
};

#endif    // MANDELLANDSCAPE_CONFIG_H
