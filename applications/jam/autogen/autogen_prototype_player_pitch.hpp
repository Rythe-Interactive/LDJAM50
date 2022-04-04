#pragma once
#include <core/types/prototype.hpp>
struct player_pitch;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::player_pitch>(const ::player_pitch& obj);
}
