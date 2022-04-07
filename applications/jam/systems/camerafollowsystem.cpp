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

        auto pos = (playerTrans.to_world_matrix() * math::vec4(follow.targetOffset, 1.f)).xyz();

        cameraPos = math::lerp(cameraPos, pos, static_cast<float>(deltaTime));
        auto maxLag = pos + math::normalize(cameraPos - pos) * follow.lagDistance;
        cameraPos = math::clamp(cameraPos, pos, maxLag);
        if (math::length(cameraPos - pos) < .1f)
            cameraPos = pos;

        auto rotTarget = rotation::lookat(pos, playerTrans.to_world_matrix() * math::vec4(math::vec3(0.f, .5f, 0.f), 1.f), playerRot.up());
        cameraRot = math::slerp(cameraRot, rotTarget, static_cast<float>(deltaTime));
        if (math::length(cameraRot - rotTarget) < .1f)
            cameraRot = rotTarget;
    }
}
