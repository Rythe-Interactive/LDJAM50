#pragma once
#include <core/types/prototype.hpp>
struct ship_comp;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::ship_comp>(const ::ship_comp& obj);
}
