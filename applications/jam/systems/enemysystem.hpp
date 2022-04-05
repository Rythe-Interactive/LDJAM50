#pragma once
#include "../engine_include.hpp"

#include "../components/components.hpp"
#include "../data/bounding_box.hpp"

using namespace lgn;
class EnemySystem final : public legion::System<EnemySystem>
{
    ecs::filter<position, rotation, scale, rigidbody, enemy_comp> enemies;
    ecs::filter<position, rotation, scale, rigidbody, player_comp> players;
    bounding_box bnds{ math::vec3(-500.f), math::vec3(500.f)};
    float border = 5.f;
    size_type bulletCount = 0;
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
    void hunt(float deltaTime);
    void shoot(ecs::entity enemy);

};
