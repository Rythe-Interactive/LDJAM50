#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include "../components/components.hpp"

using namespace lgn;
class CameraFollowSystem final : public legion::System<CameraFollowSystem>
{
    ecs::filter<position, rotation, scale, rendering::camera, camera_follow> cameras;
public:
    void setup();
    void fixedUpdate(lgn::time::span);
    void shutdown()
    {
        lgn::log::debug("CameraFollowSystem shutdown");
    }
};
