#pragma once
#include <core/types/reflector.hpp>
struct player_roll;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_roll>(::player_roll& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_roll>(const ::player_roll& obj);
}
