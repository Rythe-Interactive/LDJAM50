#pragma once
#include <core/types/reflector.hpp>
struct player_pitch;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_pitch>(::player_pitch& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_pitch>(const ::player_pitch& obj);
}
