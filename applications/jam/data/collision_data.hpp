#pragma once
#include "../engine_include.hpp"

#include "../components/collider.hpp"

using namespace lgn;

struct [[no_reflect]] collision_pair
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

    NO_DEF_CTOR_RULE5_NOEXCEPT(collision_pair);

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

struct [[no_reflect]] collision_normal
{
    math::vec3 axis;
    float depth;
};

struct [[no_reflect]] collision : events::event<collision>
{
    ecs::entity first;
    ecs::entity second;
    pointer<collider> firstCollider;
    pointer<collider> secondCollider;
    collision_normal normal;
};
