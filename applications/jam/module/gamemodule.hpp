#pragma once
#include <core/core.hpp>
#include <physics/physics.hpp>

#include "../systems/gamesystem.hpp"
#include "../systems/bulletsystem.hpp"
#include "../systems/gui_test.hpp"

class GameModule : public legion::Module
{
public:
    virtual void setup() override
    {
        using namespace legion;
        app::WindowSystem::requestWindow(
            ecs::world_entity_id,
            math::ivec2(1920, 1080),
            "LEGION Engine",
            "Legion Icon",
            nullptr,
            nullptr,
            0
        );

        reportSystem<physics::PhysicsSystem>();
        reportSystem<GameSystem>();
        reportSystem<BulletSystem>();
        reportSystem<GuiTestSystem>();
    }
};
