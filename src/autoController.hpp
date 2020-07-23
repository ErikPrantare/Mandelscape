#ifndef MANDELLANDSCAPE_AUTO_CONTROLLER_HPP
#define MANDELLANDSCAPE_AUTO_CONTROLLER_HPP

#include "utils.hpp"
#include "player.hpp"
#include "playerController.hpp"

#include <glm/glm.hpp>

#include <functional>

class AutoController final : public PlayerController {
public:
    AutoController(std::function<double(glm::dvec2)>);

    auto
    handleEvent(Event const&) -> void final{};

    auto
    update(Player&, double) -> void final;

private:
    auto
    locateTarget(Player& player) -> void;

    std::function<double(glm::dvec2)> m_heightFunc;

    static auto constexpr maxTravelTime     = 20.0;
    static auto constexpr minimumTravelTime = 7.0;
    static auto constexpr travelSpeed       = 1.0;

    glm::dvec2 m_target          = glm::dvec2{0.0, 0.0};
    double m_prevTargetDirection = 0.0;

    util::LowPassFilter<double> m_filteredLookAt{0.0, 0.99};
};

#endif
