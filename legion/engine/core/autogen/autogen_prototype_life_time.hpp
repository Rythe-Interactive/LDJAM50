#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct life_time;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::life_time>(const legion::core::life_time& obj);
}
