#pragma once
#include "../engine_include.hpp"

#include "../components/collider.hpp"

using namespace lgn;

struct [[no_reflect]] collision_pair
{
    ecs::entity                      first;
    math::mat4                       firstTransf;
    math::mat3                       firstInvRot;
    std::reference_wrapper<collider> firstCollider;

    ecs::entity                      second;
    math::mat4                       secondTransf;
    math::mat3                       secondInvRot;
    std::reference_wrapper<collider> secondCollider;

    NO_DEF_CTOR_RULE5_NOEXCEPT(collision_pair);

    collision_pair(
        const ecs::entity& _first,
        const math::mat4& _firstTransf,
        const math::mat3& _firstInvRot,
        const std::reference_wrapper<collider>& _firstCollider,
        const ecs::entity& _second,
        const math::mat4& _secondTransf,
        const math::mat3& _secondInvRot,
        const std::reference_wrapper<collider>& _secondCollider)
        :
        first(_first),
        firstTransf(_firstTransf),
        firstInvRot(_firstInvRot),
        firstCollider(_firstCollider),
        second(_second),
        secondTransf(_secondTransf),
        secondInvRot(_secondInvRot),
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
