#pragma once
#include <core/types/prototype.hpp>
struct collision_pair;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::collision_pair>(const ::collision_pair& obj);
}
