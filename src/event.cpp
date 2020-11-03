#include "event.hpp"

#include <tuple>

auto
operator<(MouseMove a, MouseMove b) -> bool
{
    return std::tie(a.x, a.y, a.dx, a.dy) < std::tie(b.x, b.y, b.dx, b.dy);
}
auto
operator<(MouseButtonUp a, MouseButtonUp b) -> bool
{
    return a.button < b.button;
}
auto
operator<(MouseButtonDown a, MouseButtonDown b) -> bool
{
    return a.button < b.button;
}
