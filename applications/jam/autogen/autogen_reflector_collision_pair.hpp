#pragma once
#include <core/types/reflector.hpp>
struct collision_pair;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::collision_pair>(::collision_pair& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::collision_pair>(const ::collision_pair& obj);
}
