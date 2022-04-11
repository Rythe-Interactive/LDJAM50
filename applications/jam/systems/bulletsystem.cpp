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
        if (ent.has_component<audio::audio_source>())
        {
            audio::audio_source& source = ent.get_component<audio::audio_source>();
            if (!source.isPlaying())
                ent.remove_component<audio::audio_source>();
        }

        bullet_comp& bullet = ent.get_component<bullet_comp>();

        bullet.age += deltaTime;
        if (bullet.age > bullet.lifetime)
        {
            ent.destroy();
            continue;
        }
        rigidbody& rb = ent.get_component<rigidbody>();
        rotation& rot = ent.get_component<rotation>();
        rb.addForce(rot.forward() * 100.f);

        scale& scal = ent.get_component<scale>();
        //scal = scal * (1.f - (bullet.age / bullet.lifetime));
        //scal = math::clamp(scal, math::vec3(0.01f), math::vec3(5.f));
    }
}
