#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct bounds;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::bounds>(legion::core::bounds& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::bounds>(const legion::core::bounds& obj);
}
