#pragma once
#include <core/types/reflector.hpp>
struct collider;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::collider>(::collider& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::collider>(const ::collider& obj);
}
