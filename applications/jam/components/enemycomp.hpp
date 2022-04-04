#pragma once
#include <core/core.hpp>
#include <core/math/math.hpp>

struct enemy_comp
{
    legion::core::id_type target = 0;
    float health = 100.f;
    float visionRadius = 8.f;
    float seperationRadius = 5.f;
    float speed = 2.f;
    legion::core::math::vec3 direction = legion::core::math::vec3::forward;
    std::vector<legion::core::id_type> neighbors;
};
