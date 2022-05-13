#pragma once
#include <core/types/reflector.hpp>
struct ship_comp;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::ship_comp>(::ship_comp& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::ship_comp>(const ::ship_comp& obj);
}
