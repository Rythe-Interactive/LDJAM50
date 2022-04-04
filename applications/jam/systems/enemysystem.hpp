#pragma once
#include <core/core.hpp>
#include <physics/physics.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include "../autogen/autogen.hpp"
#include "../components/components.hpp"

using namespace lgn;
class EnemySystem final : public legion::System<EnemySystem>
{
    ecs::filter<enemy_comp> bullets;
public:
    void setup();
    void update(lgn::time::span);
    void shutdown()
    {
        lgn::log::debug("BulletSystem shutdown");
    }
};
