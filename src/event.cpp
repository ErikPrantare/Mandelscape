#include "event.hpp"

#include <tuple>

bool
operator<(MouseMove a, MouseMove b)
{
    return std::tie(a.x, a.y, a.dx, a.dy) < std::tie(b.x, b.y, b.dx, b.dy);
}
bool
operator<(MouseButtonUp a, MouseButtonUp b)
{
    return a.button < b.button;
}
bool
operator<(MouseButtonDown a, MouseButtonDown b)
{
    return a.button < b.button;
}
