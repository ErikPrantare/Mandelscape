#include "playerHelper.hpp"

PlayerHelper::PlayerHelper(Player& player) noexcept : m_player(&player)
{}

auto
PlayerHelper::truePosition() const noexcept -> glm::dvec3
{
    return m_player->position + m_player->positionOffset;
}

auto
PlayerHelper::updateOffset(glm::dvec3 offset) noexcept -> void
{
    auto dOffset = offset - m_player->positionOffset;
    m_player->position -= dOffset;
    m_player->positionOffset = offset;
}
