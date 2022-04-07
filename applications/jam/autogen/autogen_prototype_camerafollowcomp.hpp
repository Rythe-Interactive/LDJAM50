#pragma once
#include <core/types/prototype.hpp>
struct camera_follow;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::camera_follow>(const ::camera_follow& obj);
}
