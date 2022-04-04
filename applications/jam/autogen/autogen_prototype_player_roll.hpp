#pragma once
#include <core/types/prototype.hpp>
struct player_roll;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::player_roll>(const ::player_roll& obj);
}
