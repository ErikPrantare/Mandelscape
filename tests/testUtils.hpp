#ifndef MANDELLANDSCAPE_TEST_UTILS_HPP
#define MANDELLANDSCAPE_TEST_UTILS_HPP

#include <iostream>

#include <glm/glm.hpp>

// namespace glm needed for ADL
namespace glm {
std::ostream&
operator<<(std::ostream& os, glm::dvec2 const& v)
{
    os << '(' << v.x << ", " << v.y << ')';
    return os;
}
std::ostream&
operator<<(std::ostream& os, glm::dvec3 const& v)
{
    os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
    return os;
}
}    // namespace glm

#endif
