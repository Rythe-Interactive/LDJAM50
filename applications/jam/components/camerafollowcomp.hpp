#pragma once
#include <core/core.hpp>

struct [[reflectable]] camera_follow
{
    legion::ecs::entity target;
    position targetOffset;
    float lagDistance = 1.f;
};
