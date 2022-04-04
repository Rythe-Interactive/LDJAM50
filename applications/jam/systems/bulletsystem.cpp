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
    }
}
