#include "config.hpp"

#include "utils.hpp"

auto
initConfig() noexcept -> Config
{
    Config conf;
    conf.set<Settings::WindowWidth>(1366);
    conf.set<Settings::WindowHeight>(768);
    conf.set<Settings::ClippingPlaneNear>(0.01);
    conf.set<Settings::ClippingPlaneFar>(150.0);
    conf.set<Settings::FOV>(util::pi / 2);
    conf.set<Settings::UseDeepShader>(false);
    conf.set<Settings::Iterations>(100);

    return conf;
}
