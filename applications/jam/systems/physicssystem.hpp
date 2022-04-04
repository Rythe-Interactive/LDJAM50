#pragma once
#include "../engine_include.hpp"

#include "../broadphase/broadphase.hpp"
#include "../components/rigidbody.hpp"
#include "../components/collider.hpp"

using namespace lgn;
class PhysicsSystem final : public System<PhysicsSystem>
{
    ecs::filter<rigidbody> m_rigidbodyQuery;

    static std::unique_ptr<BroadPhase> m_boardPhase;

public:
    void setup();

    void fixedUpdate(time::time_span<fast_time> deltaTime);

    void checkCollisions(float deltaTime);

    bool checkCollision(const collision_pair& pair);

    void integrateRigidbodies(float deltaTime);
};
