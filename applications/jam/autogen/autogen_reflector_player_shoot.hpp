#pragma once
#include <core/types/reflector.hpp>
struct player_shoot;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_shoot>(::player_shoot& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_shoot>(const ::player_shoot& obj);
}
