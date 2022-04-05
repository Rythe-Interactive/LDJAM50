#include "enemysystem.hpp"
#include "gui_test.hpp"
#include <rendering/debugrendering.hpp>
#include "../components/components.hpp"

void EnemySystem::setup()
{

}

void EnemySystem::update(lgn::time::span dt)
{
    locomotion(dt);
    alignment();
    cohesion();
    seperation();
    hunt(dt);
}

void EnemySystem::locomotion(float deltaTime)
{

    for (auto& ent : enemies)
    {
        auto& enemy = ent.get_component<enemy_comp>().get();
        auto& pos = ent.get_component<position>().get();
        auto& rb = ent.get_component<rigidbody>().get();
        auto& rot = ent.get_component<rotation>().get();
        auto& vel = rb.velocity;
        auto& speed = enemy.speed;
        auto& steering = enemy.direction;

        if (pos.x < bnds.min.x + border && vel.x < 0 && steering.x < 0)
            steering.x *= -1.f;
        if (pos.x > bnds.max.x - border && vel.x > 0 && steering.x > 0)
            steering.x *= -1.f;

        if (pos.y < bnds.min.y + border && vel.y < 0 && steering.y < 0)
            steering.y *= -1.f;
        if (pos.y > bnds.max.y - border && vel.y > 0 && steering.y > 0)
            steering.y *= -1.f;

        if (pos.z < bnds.min.z + border && vel.z < 0 && steering.z < 0)
            steering.z *= -1.f;
        if (pos.z > bnds.max.z - border && vel.z > 0 && steering.z > 0)
            steering.z *= -1.f;

        auto randpoint = math::diskRand(math::atan(22.5f));
        steering += math::normalize(vel + rot * math::vec3(randpoint.x, randpoint.y, 0.f)) * speed;
        vel = math::clamp(math::lerp(vel, steering, deltaTime), -velocity{ 10.f }, velocity{ 10.f });
        rb.addForce(vel);
        rot = math::quatLookAt(math::normalize(vel), math::vec3::up);
        steering = vel;

        //Boundary Teleportation
        if (pos.x < bnds.min.x)
            pos.x = bnds.max.x - border;
        if (pos.x > bnds.max.x)
            pos.x = bnds.min.x + border;

        if (pos.y < bnds.min.y)
            pos.y = bnds.max.y - border;
        if (pos.y > bnds.max.y)
            pos.y = bnds.min.y + border;

        if (pos.z < bnds.min.z)
            pos.z = bnds.max.z - border;
        if (pos.z > bnds.max.z)
            pos.z = bnds.min.z + border;

        enemy.neighbors.clear();
        for (auto ent2 : enemies)
        {
            if (ent2 == ent)
                continue;

            if (math::length2(ent2.get_component<position>().get() - pos) < enemy.visionRadius * enemy.visionRadius)
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
        auto& rb = ent.get_component<rigidbody>().get();
        auto rot = ent.get_component<rotation>();
        auto& vel = rb.velocity;
        auto& speed = enemy.speed;
        auto& steering = enemy.direction;

        size_type neighborCount = enemy.neighbors.size();
        if (neighborCount == 0)
            continue;

        velocity force{ 0.f };
        for (size_type neighbor = 0; neighbor < neighborCount; neighbor++)
            force += ecs::Registry::getEntity(enemy.neighbors[neighbor]).get_component<rigidbody>()->velocity;

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
        auto& rb = ent.get_component<rigidbody>().get();
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
        auto& rb = ent.get_component<rigidbody>().get();
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
            if (math::length2(diff) < enemy.seperationRadius * enemy.seperationRadius)
                force += enemy.seperationRadius / diff;
        }
        steering += force / neighborCount;
    }
}
void EnemySystem::hunt(float deltaTime)
{
    for (auto& player : players)
    {
        auto playerPos = player.get_component<position>().get();
        bnds.set_origin(playerPos);

        for (auto& enemy : enemies)
        {
            auto diff = enemy.get_component<position>().get() - playerPos;
            auto& enemy_c = enemy.get_component<enemy_comp>().get();
            enemy_c.elapsedTime += deltaTime;
            auto seperationRadius = enemy_c.playerSeperationRadius * enemy_c.playerSeperationRadius;
            auto huntRadius = enemy_c.playerHuntRadius * enemy_c.playerHuntRadius;
            if (math::length2(diff) < huntRadius && !enemy_c.running)
            {
                enemy_c.hunt = true;
            }

            if (math::length2(diff) < seperationRadius && enemy_c.hunt)
            {
                enemy_c.hunt = false;
                enemy_c.running = true;
            }

            if (math::length2(diff) > huntRadius)
            {
                enemy_c.hunt = false;
                enemy_c.running = false;
            }

            if (enemy_c.hunt)
            {
                enemy_c.direction = math::normalize(-diff) * enemy_c.speed * 5.f;
                if (180.f - math::rad2deg(math::angleBetween(enemy.get_component<rotation>()->forward(), math::normalize(diff))) < 15.f)
                    if (enemy_c.elapsedTime > enemy_c.shootInterval)
                    {
                        shoot(enemy);
                        enemy_c.elapsedTime = 0.f;
                    }
            }

            if (enemy_c.running)
                enemy_c.direction += seperationRadius / diff;
        }
    }
}

void EnemySystem::shoot(ecs::entity enemy)
{
    using namespace lgn;

    auto bullet = createEntity();

    auto& light = bullet.add_component<gfx::light>(gfx::light::point(math::colors::red, 2.f, 5.f)).get();
    auto& e_pos = enemy.get_component<position>().get();
    auto& e_rot = enemy.get_component<rotation>().get();
    auto [b_pos, b_rot, b_scal] = bullet.add_component<transform>();
    b_pos = e_pos.xyz() + e_rot.forward();
    b_rot = e_rot;
    b_scal = scale(.2f, .2f, 1.f);

    auto model = gfx::ModelCache::get_handle("Bullet");
    auto material = gfx::MaterialCache::get_material("Light");
    material.set_param("color", math::colors::red);
    material.set_param("intensity", 2.f);
    bullet.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ material, model });

    bullet.add_component<bullet_comp>()->fromPlayer = false;
    auto p_vel = enemy.get_component<rigidbody>()->velocity;
    auto& b_rb = bullet.add_component<rigidbody>().get();
    b_rb.velocity = p_vel;
    b_rb.setMass(.1f);

    auto& col = bullet.add_component<collider>().get();
    col.add_shape<SphereCollider>();
}

