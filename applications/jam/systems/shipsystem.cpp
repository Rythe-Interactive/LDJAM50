#include "../systems/shipsystem.hpp"
#include "../debug_utils.hpp"

void ShipSystem::setup()
{
    using namespace lgn;
    log::debug("ShipSystem setup");

    for (ecs::entity ent : ships)
    {
        ship = ent;
    }

    for (ecs::entity ent : cameras)
    {
        camera = ent;
    }
    createProcess<&ShipSystem::fixedUpdate>("Update", 0.02f);
}


void ShipSystem::fixedUpdate(lgn::time::span deltaTime)
{
    using namespace lgn;

    transform playerTrans = ship.get_component<transform>();
    position& shipPos = ship.get_component<position>();
    rigidbody& shipRb = ship.get_component<rigidbody>();
    rotation& shipRot = ship.get_component<rotation>();

    transform cameraTrans = camera.get_component<transform>();
    position& cameraPos = camera.get_component<position>();
    rotation& cameraRot = camera.get_component<rotation>();

    auto targetRot = rotation::lookat(cameraPos, cameraTrans.to_world_matrix() * math::vec4(math::vec3(0.f, 0.f, 10.f), 1.f), cameraRot.up());
    shipRot = math::slerp(shipRot, targetRot, static_cast<float>(deltaTime) * 4.f);
    if (math::length(shipRot - targetRot) < .02f)
        shipRot = targetRot;

    auto targetPos = (cameraTrans.to_world_matrix() * math::vec4(math::vec3(0.f, 0.f, 10.f), 1.f)).xyz();
    shipPos = math::lerp(shipPos, targetPos + (cameraTrans.to_world_matrix() * math::vec4(math::vec3(0.f, -2.f, -2.f), 1.f)).xyz(), static_cast<float>(deltaTime) * 2.f);
    if (math::length(shipPos - targetPos) < .02f)
        shipPos = targetPos;
    else
    {
        shipPos = targetPos + math::normalize(shipPos - targetPos) * math::clamp(math::distance(shipPos, targetPos), 0.f, 2.0f);
    }
}
