#pragma once
#include <core/types/reflector.hpp>
struct camera_follow;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::camera_follow>(::camera_follow& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::camera_follow>(const ::camera_follow& obj);
}
