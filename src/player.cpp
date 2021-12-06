#include "player.hpp"

#include <tuple>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

auto
Player::truePosition() const noexcept -> glm::dvec3
{
    return m_state.position + m_state.offset;
}

auto
Player::updateOffset(glm::dvec3 offset) noexcept -> void
{
    auto dOffset = offset - m_state.offset;
    m_state.position -= dOffset;
    m_state.offset = offset;
}

auto
Player::scale() const noexcept -> double
{
    return m_state.scale;
}

auto
Player::update(double dt) -> void
{
    m_headAltitude.update(m_state.position.y + m_state.scale, dt);
}

auto
Player::updateFeetAltitude(double altitude) noexcept -> void
{
    m_state.position.y = altitude;
}

auto
Player::getCamera(glm::ivec2 viewSize) const noexcept -> Camera
{
    glm::dvec3 cameraPosition = m_state.position;
    cameraPosition.y += m_state.scale;
    static util::LowPassFilter filteredHeight(cameraPosition.y, 0.01);
    cameraPosition.y = m_headAltitude.get();

    // + util::pi, because -z is regarded as the default lookAt forward
    auto const lookAt = glm::rotate(
                                glm::dmat4(1.0),
                                m_state.lookAtOffset.x + util::pi,
                                {0.0, 1.0, 0.0})
                        * glm::rotate(
                                glm::dmat4(1.0),
                                m_state.lookAtOffset.y,
                                {1.0, 0.0, 0.0})
                        * glm::dvec4(0.0, 0.0, 1.0, 0.0);

    // CPP20 {.x = ...}
    return Camera({cameraPosition, lookAt, viewSize, m_state.scale});
}
