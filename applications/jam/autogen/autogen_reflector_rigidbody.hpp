#pragma once
#include <core/types/reflector.hpp>
struct rigidbody;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::rigidbody>(::rigidbody& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::rigidbody>(const ::rigidbody& obj);
}
