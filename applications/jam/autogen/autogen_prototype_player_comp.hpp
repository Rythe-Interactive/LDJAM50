#pragma once
#include <core/types/prototype.hpp>
struct player_comp;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::player_comp>(const ::player_comp& obj);
}
