#pragma once
#include "../engine_include.hpp"

#include "../components/components.hpp"
#include "../defaults/defaultpolicies.hpp"

using namespace lgn;
class EnemySystem final : public legion::System<EnemySystem>
{
    ecs::filter<position, rotation, scale, rigidbody, enemy_comp> enemies;
    ecs::filter<position, rotation, scale, rigidbody, player_comp> players;
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
