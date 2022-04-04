#pragma once
#include <core/types/prototype.hpp>
struct simplex;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::simplex>(const ::simplex& obj);
}
