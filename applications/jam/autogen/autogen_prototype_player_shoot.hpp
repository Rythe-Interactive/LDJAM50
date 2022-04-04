#pragma once
#include <core/types/prototype.hpp>
struct player_shoot;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::player_shoot>(const ::player_shoot& obj);
}
