#include "player.hpp"

#include <tuple>

auto
operator==(Player const& a, Player const& b) -> bool
{
    return std::tie(a.position, a.offset, a.lookAtOffset, a.scale)
           == std::tie(b.position, b.offset, b.lookAtOffset, b.scale);
}
