#pragma once
#include <core/types/prototype.hpp>
struct player_thrust;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::player_thrust>(const ::player_thrust& obj);
}
