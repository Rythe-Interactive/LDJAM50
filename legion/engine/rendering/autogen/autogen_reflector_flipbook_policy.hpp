#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct flipbook_policy;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::flipbook_policy>(legion::rendering::flipbook_policy& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::flipbook_policy>(const legion::rendering::flipbook_policy& obj);
}
