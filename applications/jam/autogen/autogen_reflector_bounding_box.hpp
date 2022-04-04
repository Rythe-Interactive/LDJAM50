#pragma once
#include <core/types/reflector.hpp>
struct bounding_box;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::bounding_box>(::bounding_box& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::bounding_box>(const ::bounding_box& obj);
}
