#include "enemysystem.hpp"

void EnemySystem::setup()
{

}

void EnemySystem::update(lgn::time::span dt)
{
    locomotion(dt);
}

void EnemySystem::locomotion(float deltaTime)
{
    for (auto& ent : enemies)
    {
        auto& enemy = ent.get_component<enemy_comp>().get();
        auto& pos = ent.get_component<position>().get();
        auto& rb = ent.get_component<physics::rigidbody>().get();
        auto& rot = ent.get_component<rotation>().get();
        auto& vel = rb.velocity;
        auto& speed = enemy.speed;
        auto& steering = enemy.direction;

        if (pos.x < bnds.min.x + bnds.border && vel.x < 0 && steering.x < 0)
            steering.x *= -1.f;
        if (pos.x > bnds.max.x - bnds.border && vel.x > 0 && steering.x > 0)
            steering.x *= -1.f;

        if (pos.y < bnds.min.y + bnds.border && vel.y < 0 && steering.y < 0)
            steering.y *= -1.f;
        if (pos.y > bnds.max.y - bnds.border && vel.y > 0 && steering.y > 0)
            steering.y *= -1.f;

        if (pos.z < bnds.min.z + bnds.border && vel.z < 0 && steering.z < 0)
            steering.z *= -1.f;
        if (pos.z > bnds.max.z - bnds.border && vel.z > 0 && steering.z > 0)
            steering.z *= -1.f;

        auto randpoint = math::diskRand(math::atan(90.f));
        steering += math::normalize(vel + rot * math::vec3(randpoint.x, randpoint.y, 0.f)) * speed;
        vel = math::clamp(math::lerp(vel, steering, deltaTime), -velocity{ 10.f }, velocity{ 10.f });
        rb.addForce(vel);
        rot = math::quatLookAt(math::normalize(vel), math::vec3::up);
        steering = vel;

        enemy.neighbors.clear();
        for (auto ent2 : enemies)
        {
            if (ent2 == ent)
                continue;

            if (math::length(ent.get_component<position>().get() - pos) < enemy.visionRadius)
            {
                enemy.neighbors.push_back(ent2->id);
            }
        }
    }
}

void EnemySystem::alignment()
{

}

void EnemySystem::cohesion()
{

}

void EnemySystem::seperation()
{

}

