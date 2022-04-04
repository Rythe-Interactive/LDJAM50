#include "enemysystem.hpp"

void EnemySystem::setup()
{

}

void EnemySystem::update(lgn::time::span dt)
{
    locomotion(dt);
    alignment();
    cohesion();
    seperation();
    hunt();
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

        auto randpoint = math::diskRand(math::atan(22.5f));
        steering += math::normalize(vel + rot * math::vec3(randpoint.x, randpoint.y, 0.f)) * speed;
        vel = math::clamp(math::lerp(vel, steering, deltaTime), -velocity{ 5.f }, velocity{ 5.f });
        rb.addForce(vel);
        rot = math::quatLookAt(math::normalize(vel), math::vec3::up);
        steering = vel;

        enemy.neighbors.clear();
        for (auto ent2 : enemies)
        {
            if (ent2 == ent)
                continue;

            if (math::length(ent2.get_component<position>().get() - pos) < enemy.visionRadius)
            {
                enemy.neighbors.push_back(ent2->id);
            }
        }
    }
}

void EnemySystem::alignment()
{
    for (auto& ent : enemies)
    {
        auto& enemy = ent.get_component<enemy_comp>().get();
        auto& pos = ent.get_component<position>().get();
        auto& rb = ent.get_component<physics::rigidbody>().get();
        auto rot = ent.get_component<rotation>();
        auto& vel = rb.velocity;
        auto& speed = enemy.speed;
        auto& steering = enemy.direction;

        size_type neighborCount = enemy.neighbors.size();
        if (neighborCount == 0)
            continue;

        velocity force{ 0.f };
        for (size_type neighbor = 0; neighbor < neighborCount; neighbor++)
            force += ecs::Registry::getEntity(enemy.neighbors[neighbor]).get_component<physics::rigidbody>()->velocity;

        steering += force / neighborCount;
        steering -= vel;
    }
}

void EnemySystem::cohesion()
{
    for (auto& ent : enemies)
    {
        auto& enemy = ent.get_component<enemy_comp>().get();
        auto& pos = ent.get_component<position>().get();
        auto& rb = ent.get_component<physics::rigidbody>().get();
        auto rot = ent.get_component<rotation>();
        auto& vel = rb.velocity;
        auto& speed = enemy.speed;
        auto& steering = enemy.direction;

        size_type neighborCount = enemy.neighbors.size();
        if (neighborCount == 0)
            continue;

        position sumPos{ 0.f };
        for (size_type neighbor = 0; neighbor < neighborCount; neighbor++)
            sumPos += ecs::Registry::getEntity(enemy.neighbors[neighbor]).get_component<position>();

        auto avgPos = sumPos / neighborCount;
        steering += avgPos - pos;
    }
}

void EnemySystem::seperation()
{
    for (auto& ent : enemies)
    {
        auto& enemy = ent.get_component<enemy_comp>().get();
        auto& pos = ent.get_component<position>().get();
        auto& rb = ent.get_component<physics::rigidbody>().get();
        auto rot = ent.get_component<rotation>();
        auto& vel = rb.velocity;
        auto& speed = enemy.speed;
        auto& steering = enemy.direction;

        size_type neighborCount = enemy.neighbors.size();
        if (neighborCount == 0)
            continue;

        velocity force{ 0.f };
        for (size_type neighbor = 0; neighbor < neighborCount; neighbor++)
        {
            auto& neighborPos = ecs::Registry::getEntity(enemy.neighbors[neighbor]).get_component<position>().get();
            auto diff = (pos - neighborPos).xyz();
            if (math::length(diff) < enemy.seperationRadius)
                force += enemy.seperationRadius / diff;
        }
        steering += force / neighborCount;
    }
}

void EnemySystem::hunt()
{
    for (auto& player : players)
    {
        for (auto& enemy : enemies)
        {
            auto diff = player.get_component<position>().get() - enemy.get_component<position>().get();
            if (math::length(diff) < enemy.get_component<enemy_comp>()->visionRadius)
            {
                enemy.get_component<enemy_comp>()->direction += diff;
            }
        }
    }
}

