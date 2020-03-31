#ifndef CONFIG_H
#define CONFIG_H

#include <type_traits>
#include <map>
#include <any>

class Config {
public:
    Config() = default;

    template<
            typename Setting,
            typename T = typename Setting::type,
            typename   = typename std::enable_if_t<
                    Setting::isSetting
                    && std::is_same_v<typename Setting::type, T>>>
    void
    set(T newValue)
    {
        m_settings[Setting::uid] = std::any(std::move(newValue));
    }

    template<
            typename Setting,
            typename ReturnType = typename Setting::type,
            typename            = typename std::enable_if_t<Setting::isSetting>>
    ReturnType
    get()
    {
        return std::any_cast<ReturnType>(m_settings[Setting::uid]);
    }

private:
    std::map<int, std::any> m_settings;
};

#endif    // CONFIG_H
