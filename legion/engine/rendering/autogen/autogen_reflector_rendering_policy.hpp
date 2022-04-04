#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct rendering_policy;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::rendering_policy>(legion::rendering::rendering_policy& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::rendering_policy>(const legion::rendering::rendering_policy& obj);
}
