#ifndef CONFIG_H
#define CONFIG_H

#include <type_traits>
#include <map>
#include <vector>
#include <any>
#include <functional>

class Config {
    using FunctionSig = std::function<void(std::any const&)>;

public:
    Config() = default;

    template<
            typename Setting,
            typename = typename std::enable_if_t<Setting::isSetting>>
    void
    set(typename Setting::type newValue)
    {
        m_settings[Setting::uid] = std::any(std::move(newValue));
        std::any const& value    = m_settings[Setting::uid];

        for(const auto& cb : m_callbacks[Setting::uid]) {
            cb(value);
        }
    }

    template<
            typename Setting,
            typename = typename std::enable_if_t<Setting::isSetting>>
    typename Setting::type
    get()
    {
        return std::any_cast<Setting::type>(m_settings[Setting::uid]);
    }

    template<
            typename Setting,
            typename Function,
            typename = typename std::enable_if_t<Setting::isSetting>>
    void
    subscribe_with(Function const callback)
    {
        m_callbacks[Setting::uid].push_back(
                std::function([callback](std::any v){
                        callback(std::any_cast<Setting::type>(v));
                    }));
    }

private:
    std::map<int, std::any> m_settings;
    std::map<int, std::vector<FunctionSig>> m_callbacks;
};

#endif    // CONFIG_H
