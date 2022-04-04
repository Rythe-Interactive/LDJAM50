#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct life_time;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::life_time>(legion::core::life_time& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::life_time>(const legion::core::life_time& obj);
}
