#pragma once
#include <core/types/reflector.hpp>
struct player_vertical;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_vertical>(::player_vertical& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_vertical>(const ::player_vertical& obj);
}
