#pragma once
#include <core/types/reflector.hpp>
struct bullet_comp;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::bullet_comp>(::bullet_comp& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::bullet_comp>(const ::bullet_comp& obj);
}
