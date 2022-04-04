#pragma once
#include <core/types/prototype.hpp>
struct bullet_comp;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::bullet_comp>(const ::bullet_comp& obj);
}
