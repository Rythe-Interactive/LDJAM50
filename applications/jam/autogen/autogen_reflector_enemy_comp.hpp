#pragma once
#include <core/types/reflector.hpp>
struct enemy_comp;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::enemy_comp>(::enemy_comp& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::enemy_comp>(const ::enemy_comp& obj);
}
