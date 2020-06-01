#ifndef MANDELLANDSCAPE_PLAYERCONTROLLER_TESTS_HPP
#define MANDELLANDSCAPE_PLAYERCONTROLLER_TESTS_HPP

#include <memory>

#include "playerController.hpp"

glm::dvec3 constexpr DUMMY_VELOCITY = {1.23, 6532936528529.363663, -0.00001};

class
DummyController : public PlayerController
{
public:
    glm::dvec3 velocity() const override {
        return DUMMY_VELOCITY;
    }
};


TEST_CASE(
        "playerController::getVelocity returns overloaded velocity",
        "[playerController]")
{
    auto dummy = std::unique_ptr<PlayerController>(new DummyController());
    
    REQUIRE(dummy->velocity() == DUMMY_VELOCITY);
}


#endif
