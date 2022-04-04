#pragma once
#include <core/core.hpp>
#include <core/math/math.hpp>

struct enemy_comp
{
    legion::core::id_type target = 0;
    float health = 100.f;
    float visionRadius = 10.f;
    float seperationRadius = 1.f;
    float speed = 20.f;
    legion::core::math::vec3 direction;
    std::vector<legion::core::id_type> neighbors;
};
