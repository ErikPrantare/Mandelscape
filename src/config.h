#ifndef MANDELLANDSCAPE_CONFIG_H
#define MANDELLANDSCAPE_CONFIG_H

#include <type_traits>
#include <map>
#include <vector>
#include <any>
#include <functional>

#include "settings.h"

class Config {
    using FunctionSig = std::function<void(std::any const&)>;
    template<typename T1, typename T2>
    using enable_if_same_t = std::enable_if_t<std::is_same_v<T1, T2>>;

public:
    Config() = default;

    template<
            typename Setting,
            typename = enable_if_same_t<
                    typename Setting::Token,
                    Settings::Secret::Token>>
    void
    set(typename Setting::Type newValue)
    {
        std::any const value     = std::any(newValue);
        m_settings[Setting::uid] = value;

        for(const auto& callback : m_callbacks[Setting::uid]) {
            callback(value);
        }
    }

    template<
            typename Setting,
            typename = enable_if_same_t<
                    typename Setting::Token,
                    Settings::Secret::Token>>
    typename Setting::Type
    get()
    {
        return std::any_cast<typename Setting::Type>(m_settings[Setting::uid]);
    }

    template<
            typename Setting,
            typename Function,
            typename = enable_if_same_t<
                    typename Setting::Token,
                    Settings::Secret::Token>>
    void
    subscribe(Function const& callback)
    {
        m_callbacks[Setting::uid].push_back(
                std::function([callback](std::any v) {
                    callback(std::any_cast<Setting::Type>(v));
                }));
    }

private:
    std::map<int, std::any> m_settings;
    std::map<int, std::vector<FunctionSig>> m_callbacks;
};

#endif    // CONFIG_H
