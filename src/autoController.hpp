#ifndef MANDELLANDSCAPE_AUTO_CONTROLLER_HPP
#define MANDELLANDSCAPE_AUTO_CONTROLLER_HPP

#include <functional>

#include <glm/glm.hpp>

#include "playerController.hpp"
#include "utils.hpp"

class AutoController : public PlayerController {
public:
    AutoController(std::function<double(glm::dvec2)>);

    auto
    handleEvent(Event const&) -> void override{};

    auto
    update(Player*, double) -> void override;

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
