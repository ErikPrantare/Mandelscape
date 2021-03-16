#include "genericController.hpp"

auto
GenericController::withMomentary(std::function<MomentarySignature> f) const
        -> GenericController
{
    GenericController newController       = *this;
    newController.m_handleMomentaryAction = std::move(f);
    return newController;
}

auto
GenericController::handleMomentaryAction(MomentaryAction const& action) const
        -> void
{
    m_handleMomentaryAction(action);
}
