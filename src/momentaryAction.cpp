#include "momentaryAction.hpp"

#include <tuple>

bool
operator<(MouseDelta a, MouseDelta b)
{
    return std::tie(a.dx, a.dy) < std::tie(b.dx, b.dy);
}
