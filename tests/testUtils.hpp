#ifndef MANDELLANDSCAPE_TEST_UTILS_HPP
#define MANDELLANDSCAPE_TEST_UTILS_HPP

#include <iostream>

#include <catch2/catch.hpp>
#include <glm/glm.hpp>

struct Dvec2Approx {
    glm::dvec2 val;
};

bool
operator==(glm::dvec2 a, Dvec2Approx b)
{
    return a.x == Approx(b.val.x) && a.y == Approx(b.val.y);
}

struct Dvec3Approx {
    glm::dvec3 val;
};

bool
operator==(glm::dvec3 a, Dvec3Approx b)
{
    return a.x == Approx(b.val.x) && a.y == Approx(b.val.y)
           && a.z == Approx(b.val.z);
}
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
