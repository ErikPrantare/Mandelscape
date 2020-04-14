#include "math3d.h"

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4
rotationMatrix(glm::vec3 const& rotation)
{
    auto const mx = glm::rotate(
            glm::mat4(1.0f),
            rotation.x,
            glm::vec3(1.0f, 0.0f, 0.0f));

    auto const my = glm::rotate(
            glm::mat4(1.0f),
            rotation.y,
            glm::vec3(0.0f, -1.0f, 0.0f));

    return my * mx;
}
