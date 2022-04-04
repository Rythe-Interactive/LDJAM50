#pragma once
#include <core/types/reflector.hpp>
struct player_thrust;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_thrust>(::player_thrust& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_thrust>(const ::player_thrust& obj);
}
