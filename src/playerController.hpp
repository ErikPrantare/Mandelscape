#ifndef MANDELLANDSCAPE_PLAYER_CONTROLLER_HPP
#define MANDELLANDSCAPE_PLAYER_CONTROLLER_HPP

#include "event.hpp"
#include "player.hpp"

#include <glm/vec2.hpp>

#include <memory>
#include <utility>

struct ControllerModel {
    virtual auto
    handleEvent(Event const& event) -> void = 0;

    virtual auto
    update(Player* player, double deltaSeconds) -> void = 0;

    ControllerModel(ControllerModel const&) = default;
    ControllerModel(ControllerModel&&)      = default;

    ControllerModel&
    operator=(ControllerModel const&) = default;
    ControllerModel&
    operator=(ControllerModel&&) = default;

    ControllerModel()          = default;
    virtual ~ControllerModel() = default;
};

template<class Controller>
struct ControllerImpl final : ControllerModel {
    ControllerImpl(Controller controller) : controller{std::move(controller)}
    {}

    auto
    handleEvent(Event const& event) -> void final
    {
        controller.handleEvent(event);
    }

    auto
    update(Player* player, double deltaSeconds) -> void final
    {
        controller.update(player, deltaSeconds);
    }

    Controller controller;
};

class PlayerController {
public:
    template<class Controller>
    PlayerController(Controller&& controller) :
                m_controller{new ControllerImpl{
                        std::forward<Controller>(controller)}}
    {}

    auto
    handleEvent(Event const& event) -> void
    {
        m_controller->handleEvent(event);
    }

    auto
    update(Player* player, double deltaSeconds) -> void
    {
        m_controller->update(player, deltaSeconds);
    }

private:
    std::unique_ptr<ControllerModel> m_controller;
};

#endif    // MANDELLANDSCAPE_PLAYER_CONTROLLER_HPP
