#pragma once
#include <core/types/prototype.hpp>
struct collider;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::collider>(const ::collider& obj);
}
