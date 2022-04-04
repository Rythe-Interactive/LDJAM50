#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct flipbook_policy;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::flipbook_policy>(const legion::rendering::flipbook_policy& obj);
}
