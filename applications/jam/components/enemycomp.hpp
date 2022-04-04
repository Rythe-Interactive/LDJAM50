#pragma once
#include <core/types/primitives.hpp>

struct [[reflectable]] enemy_comp
{
    legion::core::id_type target;
    float health;
};
