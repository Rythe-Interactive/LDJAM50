#pragma once
#include <core/types/reflector.hpp>
struct player_yaw;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_yaw>(::player_yaw& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_yaw>(const ::player_yaw& obj);
}
