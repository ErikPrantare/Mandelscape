#include "math3d.h"

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

float
dot(glm::vec3 const& a, glm::vec3 const& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

glm::vec3
cross(glm::vec3 const& a, glm::vec3 const& b)
{
    return glm::vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x);
}

glm::vec3
normalize(glm::vec3 const& x)
{
    return (1 / std::sqrt(dot(x, x))) * x;
}

glm::mat4
translationMatrix(glm::vec3 const& translation)
{
    return glm::transpose(glm::mat4(
            {1.0f, 0.0f, 0.0f, translation.x},
            {0.0f, 1.0f, 0.0f, translation.y},
            {0.0f, 0.0f, 1.0f, translation.z},
            {0.0f, 0.0f, 0.0f, 1.0f}));
}

glm::mat4
scaleMatrix(glm::vec3 const& scale)
{
    return glm::transpose(glm::mat4(
            {scale.x, 0.0f, 0.0f, 0.0f},
            {0.0f, scale.y, 0.0f, 0.0f},
            {0.0f, 0.0f, scale.z, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}));
}

glm::mat4
rotationMatrix(glm::vec3 const& rotation)
{
    auto const mx = glm::transpose(glm::mat4(
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, std::cos(rotation.x), -std::sin(rotation.x), 0.0f},
            {0.0f, std::sin(rotation.x), std::cos(rotation.x), 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}));

    auto const my = glm::transpose(glm::mat4(
            {std::cos(rotation.y), 0.0f, -std::sin(rotation.y), 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {std::sin(rotation.y), 0.0f, std::cos(rotation.y), 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}));

    auto const mz = glm::transpose(glm::mat4(
            {std::cos(rotation.z), -std::sin(rotation.z), 0.0f, 0.0f},
            {std::sin(rotation.z), std::cos(rotation.z), 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}));

    return mz * my * mx;
}

float
toRadians(float x)
{
    return x * pi / 180;
}

float
toDegrees(float x)
{
    return x / pi * 180;
}
