#ifndef MANDELLANDSCAPE_AUTO_CONTROLLER_HPP
#define MANDELLANDSCAPE_AUTO_CONTROLLER_HPP

#include <functional>

#include <glm/glm.hpp>

#include "playerController.hpp"

class AutoController : public PlayerController {
public:
    AutoController(std::function<double(glm::dvec2)>);

    auto
    handleEvent(Event const&) -> void override{};
    auto
    update(Player*, double) -> void override;

private:
    std::function<double(glm::dvec2)> m_heightFunc;
    bool m_targetFound  = false;
    glm::dvec2 m_target = glm::dvec2{0.0, 0.0};
};

#endif
