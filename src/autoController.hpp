#ifndef MANDELLANDSCAPE_AUTO_CONTROLLER_HPP
#define MANDELLANDSCAPE_AUTO_CONTROLLER_HPP

#include "utils.hpp"
#include "player.hpp"

#include <glm/glm.hpp>

#include <functional>

class AutoController {
public:
    AutoController(std::function<double(glm::dvec2)>);

    auto
    handleEvent(Event const&) -> void{};

    auto
    update(Player*, double) -> void;

    auto
    refresh() -> void;

private:
    auto
    locateTarget(Player* player) -> void;

    std::function<double(glm::dvec2)> m_heightFunc;

    bool m_hasTarget             = false;
    glm::dvec2 m_target          = glm::dvec2{0.0, 0.0};
    double m_prevTargetDirection = 0.0;

    util::LowPassFilter<double> m_filteredLookAt{0.0, 0.99};
};

#endif
