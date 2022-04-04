#pragma once
#include <core/types/prototype.hpp>
struct player_yaw;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::player_yaw>(const ::player_yaw& obj);
}
