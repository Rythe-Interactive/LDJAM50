#pragma once
#include <core/core.hpp>
#include <physics/physics.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include "../autogen/autogen.hpp"
#include "../components/components.hpp"
#include "../defaults/defaultpolicies.hpp"

using namespace lgn;
class EnemySystem final : public legion::System<EnemySystem>
{
    ecs::filter<position, rotation, scale, physics::rigidbody, enemy_comp> enemies;
    ecs::filter<position, rotation, scale, physics::rigidbody, player_comp> players;
    bounds bnds{ -25.f,25.f, 5.f };
public:
    void setup();
    void update(lgn::time::span);
    void shutdown()
    {
        lgn::log::debug("EnemySystem shutdown");
    }

    void locomotion(float deltaTime);
    void alignment();
    void cohesion();
    void seperation();
    void hunt();

};
