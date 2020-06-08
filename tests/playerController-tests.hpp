#ifndef MANDELLANDSCAPE_PLAYERCONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_PLAYERCONTROLLER_TESTS_HPP

#include <memory>

#include "event.hpp"

#include "playerController.hpp"

namespace PlayerControllerTest {

glm::dvec3 constexpr G_DUMMY_INIT = {1.23, 6532936528529.363663, -0.00001};
glm::dvec3 constexpr G_POST_EVENT = {666666666, -0.363663, -1.0 / 3.0};

class DummyController : public PlayerController {
public:
    glm::dvec3
    velocity() const override
    {
        return m_val;
    }

    void
    handleEvent(Event const&) override
    {
        m_val = G_POST_EVENT;
    }

private:
    glm::dvec3 m_val = G_DUMMY_INIT;
};

TEST_CASE(
        "playerController::getVelocity returns overloaded velocity",
        "[PlayerController]")
{
    auto controller = std::unique_ptr<PlayerController>(new DummyController());

    REQUIRE(controller->velocity() == G_DUMMY_INIT);

    controller->handleEvent(MouseButtonDown{});

    REQUIRE(controller->velocity() == G_POST_EVENT);
}

}    // namespace PlayerControllerTest

#endif
