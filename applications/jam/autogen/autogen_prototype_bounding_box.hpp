#pragma once
#include <core/types/prototype.hpp>
struct bounding_box;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::bounding_box>(const ::bounding_box& obj);
}
