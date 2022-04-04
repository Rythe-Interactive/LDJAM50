#pragma once
#include <core/types/reflector.hpp>
struct simplex;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::simplex>(::simplex& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::simplex>(const ::simplex& obj);
}
