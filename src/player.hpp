#ifndef MANDELLANDSCAPE_PLAYER_HPP
#define MANDELLANDSCAPE_PLAYER_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "event.hpp"
#include "util.hpp"

struct Player {
    glm::dvec3 position       = glm::dvec3{0.0, 0.0, 0.0};
    glm::dvec3 positionOffset = glm::dvec3{0.0, 0.0, 0.0};
    glm::dvec2 lookAtOffset   = glm::dvec2{0.0, 0.0};
    double scale              = 1.0;
};

#endif
