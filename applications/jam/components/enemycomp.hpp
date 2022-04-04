#pragma once
#include <core/types/primitives.hpp>

struct enemy_comp
{
    legion::core::id_type target;
    float health;
};
