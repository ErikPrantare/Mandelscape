#ifndef MANDELLANDSCAPE_AUTO_CONTROLLER_HPP
#define MANDELLANDSCAPE_AUTO_CONTROLLER_HPP

#include "playerController.hpp"

class AutoController : public PlayerController {
public:
    auto
    handleEvent(Event const&) -> void override{};
    auto
    update(Player*, double) -> void override;
};

#endif
