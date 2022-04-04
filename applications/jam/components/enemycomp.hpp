#pragma once
#include <core/core.hpp>
#include <core/math/math.hpp>

struct [[reflectable]] enemy_comp
{
    legion::core::id_type target = 0;
    float health = 100.f;
    float visionRadius = 6.f;
    float playerHuntRadius = 40.f;
    float playerSeperationRadius = 4.f;
    float seperationRadius = 5.f;
    float speed = 2.f;
    bool hunt = false;
    bool running = false;
    float elapsedTime = 0.f;
    float shootInterval = 1.f;
    legion::core::math::vec3 direction = legion::core::math::vec3::forward;
    std::vector<legion::core::id_type> neighbors;
};
