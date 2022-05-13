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

        ecs::entity ship = follow.target;
        position playerPos = ship.get_component<position>();
        rigidbody playerRb = ship.get_component<rigidbody>();
        transform playerTrans = ship.get_component<transform>();
        transform cameraTrans = camera.get_component<transform>();
        position& cameraPos = camera.get_component<position>();
        rotation& cameraRot = camera.get_component<rotation>();
        rotation& playerRot = ship.get_component<rotation>();

        auto targetRot = rotation::lookat(cameraPos, playerTrans.to_world_matrix() * math::vec4(follow.lookAtOffset, 1.f), playerRot.up());
        cameraRot = math::slerp(cameraRot, targetRot, static_cast<float>(deltaTime) * 7.f);
        if (math::length(cameraRot - targetRot) < .02f)
            cameraRot = targetRot;

        auto targetPos = (playerTrans.to_world_matrix() * math::vec4(follow.targetOffset, 1.f)).xyz();
        cameraPos = math::lerp(cameraPos, targetPos, static_cast<float>(deltaTime) * 4.f);
        if (math::length(cameraPos - targetPos) < .02f)
            cameraPos = targetPos;
        else
            cameraPos = targetPos + math::normalize(cameraPos - targetPos) * math::clamp(math::distance(cameraPos, targetPos), 0.f, follow.lagDistance);
    }
}
