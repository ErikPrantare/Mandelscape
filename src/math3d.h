#ifndef MANDELLANDSCAPE_MATH3D_H
#define MANDELLANDSCAPE_MATH3D_H

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

long double constexpr pi = glm::pi<long double>();

float
dot(glm::vec3 const&, glm::vec3 const&);

glm::vec3
cross(glm::vec3 const&, glm::vec3 const&);

glm::vec3
normalize(glm::vec3 const&);

glm::mat4
translationMatrix(glm::vec3 const&);

glm::mat4
scaleMatrix(glm::vec3 const&);

glm::mat4
rotationMatrix(glm::vec3 const&);

float
toRadians(float);

float
toDegrees(float);

#endif
