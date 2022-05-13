#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include "../components/components.hpp"

using namespace lgn;
class ShipSystem final : public legion::System<ShipSystem>
{
    ecs::filter<position, rotation, scale, rigidbody, ship_comp > ships;
    ecs::filter<position, rotation, scale, rendering::camera, rigidbody> cameras;

    ecs::entity ship;
    ecs::entity camera;
public:
    void setup();
    void fixedUpdate(lgn::time::span);
    void shutdown()
    {
        lgn::log::debug("ShipSystem shutdown");
    }
};
