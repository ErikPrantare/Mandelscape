#include "player.hpp"

#include <algorithm>
#include <functional>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <GLFW/glfw3.h>

#include "event.hpp"
#include "utils.hpp"
#include "mandelTypeTraits.hpp"

void
Player::update(glm::dvec2 const& terrainOffset, double /*dt*/)
{
    auto dPos       = terrainOffset - m_terrainOffset;
    m_terrainOffset = terrainOffset;
    position.x -= dPos.x;
    position.z -= dPos.y;
}
