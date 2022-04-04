#pragma once
#include <core/types/prototype.hpp>
struct player_strafe;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::player_strafe>(const ::player_strafe& obj);
}
