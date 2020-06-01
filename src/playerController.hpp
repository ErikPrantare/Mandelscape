#ifndef MANDELLANDSCAPE_PLAYERCONTROLLER_HPP
#define MANDELLANDSCAPE_PLAYERCONTROLLER_HPP

#include <glm/glm.hpp>

class
PlayerController
{
public:
    auto virtual velocity() const -> glm::dvec3 = 0;
};


#endif
