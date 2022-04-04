#pragma once
#include "../engine_include.hpp"

#include "../components/collider.hpp"

using namespace lgn;

struct collision_pair
{
    ecs::entity                      first;
    math::vec3                       firstPosition;
    math::quat                       firstRotation;
    math::vec3                       firstScale;
    std::reference_wrapper<collider> firstCollider;

    ecs::entity                      second;
    math::vec3                       secondPosition;
    math::quat                       secondRotation;
    math::vec3                       secondScale;
    std::reference_wrapper<collider> secondCollider;

    RULE_OF_5_NOEXCEPT(collision_pair);

    collision_pair(
        const ecs::entity& _first,
        const math::vec3& _firstPosition,
        const math::quat& _firstRotation,
        const math::vec3& _firstScale,
        const std::reference_wrapper<collider>& _firstCollider,
        const ecs::entity& _second,
        const math::vec3& _secondPosition,
        const math::quat& _secondRotation,
        const math::vec3& _secondScale,
        const std::reference_wrapper<collider>& _secondCollider)
        :
        first(_first),
        firstPosition(_firstPosition),
        firstRotation(_firstRotation),
        firstScale(_firstScale),
        firstCollider(_firstCollider),
        second(_second),
        secondPosition(_secondPosition),
        secondRotation(_secondRotation),
        secondScale(_secondScale),
        secondCollider(_secondCollider)
    {}
};
