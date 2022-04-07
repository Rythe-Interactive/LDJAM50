#include "../systems/camerafollowsystem.hpp"
#include "../debug_utils.hpp"

void CameraFollowSystem::setup()
{
    using namespace lgn;
    log::debug("CameraFollowSystem setup");

    for (auto& camera : cameras)
    {
        gfx::camera& cam = camera.get_component<gfx::camera>();
        camera_follow& follow = camera.get_component<camera_follow>();
        position& cameraPos = camera.get_component<position>();
        if (!follow.target)
            continue;

        position playerPos = follow.target.get_component<position>();

        cameraPos = playerPos + follow.targetOffset;
    }

    createProcess<&CameraFollowSystem::fixedUpdate>("Update", 0.02f);
}

void CameraFollowSystem::fixedUpdate(lgn::time::span deltaTime)
{
    using namespace lgn;
    for (auto& camera : cameras)
    {
        gfx::camera& cam = camera.get_component<gfx::camera>();
        camera_follow& follow = camera.get_component<camera_follow>();

        if (!follow.target)
            continue;
        ecs::entity player = follow.target;
        position playerPos = player.get_component<position>();
        rigidbody playerRb = player.get_component<rigidbody>();
        transform playerTrans = player.get_component<transform>();
        transform cameraTrans = camera.get_component<transform>();
        position& cameraPos = camera.get_component<position>();
        rotation& cameraRot = camera.get_component<rotation>();
        rotation& playerRot = player.get_component<rotation>();

        auto pos = playerTrans.to_world_matrix() * math::vec4(follow.targetOffset, 1.f);
        auto maxLag = pos.xyz() + math::normalize(cameraPos - pos.xyz()) * follow.lagDistance;
        cameraPos += playerRb.velocity * static_cast<float>(deltaTime) * 2.f;
        cameraPos = math::clamp(cameraPos, pos.xyz(), maxLag);
        cameraRot = rotation::lookat(pos, playerTrans.to_world_matrix() * math::vec4(0.f, .8f, 0.f, 1.f));
    }
}
