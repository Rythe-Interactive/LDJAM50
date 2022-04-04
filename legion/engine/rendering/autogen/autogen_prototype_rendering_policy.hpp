#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct rendering_policy;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::rendering_policy>(const legion::rendering::rendering_policy& obj);
}
