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
        position& playerPos = player.get_component<position>();
        bnds.set_origin(playerPos);

        for (auto& enemy : enemies)
        {
            position& enemyPos = enemy.get_component<position>();
            auto diff = enemyPos - playerPos;
            enemy_comp& enemy_c = enemy.get_component<enemy_comp>();
            enemy_c.elapsedTime += deltaTime;
            auto seperationRadius = enemy_c.playerSeperationRadius * enemy_c.playerSeperationRadius;
            auto visionRadius = enemy_c.visionRadius * enemy_c.visionRadius;

            if (math::length2(diff) < seperationRadius)
            {
                enemy_c.running = true;
            }

            if (math::length2(diff) > visionRadius)
            {
                enemy_c.running = false;
            }

            if (!enemy_c.running)
            {
                enemy_c.direction = math::normalize(-diff) * enemy_c.speed;
                debug::drawLine(enemyPos, enemyPos + enemy_c.direction);
                if (180.f - math::rad2deg(math::angleBetween(enemy.get_component<rotation>()->forward(), math::normalize(diff))) < 15.f)
                    if (enemy_c.elapsedTime > enemy_c.shootInterval)
                    {
                        shoot(enemy);
                        enemy_c.elapsedTime = 0.f;
                    }
            }
            else if (enemy_c.running)
                enemy_c.direction += seperationRadius / diff;
        }
    }
}

void EnemySystem::shoot(ecs::entity enemy)
{
    using namespace lgn;

    static size_type i = 0;
    auto bullet = createEntity("EnemyBullet" + std::to_string(i++));
    bullet.add_component<transform>();
    bullet.add_component<rigidbody>();
    bullet.add_component<bullet_comp>();
    bullet.add_component<gfx::light>(gfx::light::point(math::colors::red, 2.f, 5.f)).get();
    bullet.add_component<audio::audio_source>(audio::AudioSegmentCache::getAudioSegment("LaserShot"));

    if (bullet.has_component<audio::audio_source>())
    {
        audio::audio_source& source = bullet.get_component<audio::audio_source>();
        source.play();
    }

    position enemyPos = enemy.get_component<position>();
    rotation enemyRot = enemy.get_component<rotation>();
    position& bulletPos = bullet.get_component<position>();
    rotation& bulletRot = bullet.get_component<rotation>();
    scale& bulletScal = bullet.get_component<scale>();
    bulletPos = enemyPos.xyz() + enemyRot.forward() * .5f;
    bulletRot = enemyRot;
    bulletScal = scale(.3f, .3f, 1.5f);

    auto model = gfx::ModelCache::get_handle("Bullet");
    auto material = gfx::MaterialCache::get_material("Light");
    material.set_param("color", math::colors::red);
    material.set_param("intensity", 2.f);
    bullet.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ material, model });

    bullet_comp& bulletComp = bullet.get_component<bullet_comp>();
    bulletComp.fromPlayer = false;

    rigidbody& bulletRb = bullet.get_component<rigidbody>();
    rigidbody enemyRb = enemy.get_component<rigidbody>();
    bulletRb.velocity = enemyRb.velocity;
    bulletRb.setMass(.1f);

    auto& col = bullet.add_component<collider>().get();
    col.layer = 2;
    col.ignoreMask = 1 | 2;
    col.add_shape<SphereCollider>();
}

