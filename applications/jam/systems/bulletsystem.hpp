#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include "../autogen/autogen.hpp"
#include "../components/components.hpp"

using namespace lgn;
class BulletSystem final : public legion::System<BulletSystem>
{
    ecs::filter<position, rotation, scale, rigidbody, bullet_comp> bullets;
public:
    void setup();
    void update(lgn::time::span);
    void shutdown()
    {
        lgn::log::debug("BulletSystem shutdown");
    }
};
