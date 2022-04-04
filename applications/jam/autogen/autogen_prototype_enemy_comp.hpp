#pragma once
#include <core/types/prototype.hpp>
struct enemy_comp;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::enemy_comp>(const ::enemy_comp& obj);
}
