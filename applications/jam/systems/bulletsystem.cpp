#include "bulletsystem.hpp"

void BulletSystem::setup()
{
    using namespace lgn;
    log::debug("BulletSystem setup");
}

void BulletSystem::update(lgn::time::span deltaTime)
{
    using namespace lgn;
    for (auto& ent : bullets)
    {
        auto& bullet = ent.get_component<bullet_comp>().get();
        bullet.age += deltaTime;
        if (bullet.age > bullet.lifetime)
        {
            ent.destroy();
            continue;
        }
        auto& rb = ent.get_component<rigidbody>().get();
        auto& rot = ent.get_component<rotation>().get();
        rb.addForce(rot.forward() * 2000.f * (float)deltaTime);

        //auto& scal = ent.get_component<scale>().get();
        //scal.z = math::length(math::normalize(rb.velocity) * 2.f);
        //scal = scal * (1.f - (bullet.age / bullet.lifetime));
        //scal = math::clamp(scal, math::vec3(0.01f), math::vec3(5.f));
    }
}
