#include "../systems/gamesystem.hpp"
#include "../components/components.hpp"
#include "../defaults/defaultpolicies.hpp"

#include <rendering/data/particlepolicies/renderpolicy.hpp>
#include <rendering/data/particlepolicies/flipbookpolicy.hpp>

#include <rendering/util/gui.hpp>
#include <rendering/pipeline/gui/stages/imguirenderstage.hpp>
#include <imgui/imgui_internal.h>

void GameSystem::setup()
{
    using namespace legion;
    log::filter(log::severity_debug);
    log::debug("GameSystem setup");

    auto* pipeline = dynamic_cast<gfx::DefaultPipeline*>(gfx::Renderer::getMainPipeline());
    if (pipeline)
    {
        pipeline->attachStage<ImGuiStage>();
        pipeline->attachStage<MouseHover>();
    }

    initInput();

    ImGuiStage::addGuiRender<GameSystem, &GameSystem::onGUI>(this);

    app::window& window = ecs::world.get_component<app::window>();
    window.enableCursor(true);
    window.show();
    app::context_guard guard(window);
    {
        auto skyboxMat = rendering::MaterialCache::create_material("skybox", "assets://shaders/skybox.shs"_view);
        skyboxMat.set_param(SV_SKYBOX, TextureCache::create_texture("planet atmo", fs::view("assets://textures/HDRI/planetatmo7.png"),
            texture_import_settings
            {
                texture_type::two_dimensional, true, channel_format::eight_bit, texture_format::rgba_hdr,
                texture_components::rgba, true, true, 0, texture_mipmap::linear_mipmap_linear, texture_mipmap::linear,
                texture_wrap::edge_clamp, texture_wrap::repeat, texture_wrap::edge_clamp
            }));
        ecs::world.add_component(gfx::skybox_renderer{ skyboxMat });

        {
            gfx::ModelCache::create_model("Particle", fs::view("assets://models/billboard.glb"));
            gfx::ModelCache::create_model("Bullet", fs::view("assets://models/sphere.obj"));
            gfx::ModelCache::create_model("Ship", fs::view("assets://models/ship/JamShip.glb"));
            gfx::ModelCache::create_model("Asteroid1", fs::view("assets://models/asteroid/JamAsteroid1.glb"));
            gfx::ModelCache::create_model("Enemy", fs::view("assets://models/ship/JamEnemy.glb"));
            gfx::MaterialCache::create_material("ShipLit", fs::view("engine://shaders/default_lit.shs"));
        }

        {
            audio::AudioSegmentCache::createAudioSegment("Explosion", fs::view("assets://audio/fx/Explosion2.wav"));
            audio::AudioSegmentCache::createAudioSegment("LaserShot", fs::view("assets://audio/fx/Laser_Shoot.wav"));
            audio::AudioSegmentCache::createAudioSegment("BGMusic", fs::view("assets://audio/background.mp3"));
        }

        {
            gfx::MaterialCache::create_material("BulletMat", fs::view("assets://shaders/texture.shs"));
            gfx::MaterialCache::get_material("BulletMat").set_param("_texture", gfx::TextureCache::create_texture("Default", fs::view("engine://resources/default/albedo")));
        }

        {
            gfx::MaterialCache::create_material("ForceField", fs::view("assets:/shaders/forcefield.shs")).set_variant("transparent");
            gfx::MaterialCache::create_material("ShipLit", fs::view("engine://shaders/default_lit.shs"));
            texture_import_settings settings = gfx::default_texture_settings;
            settings.mag = gfx::texture_mipmap::nearest;
            auto color = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/ColorPalette.png"), settings);
            auto emissive = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/EmissivePallete.png"), settings);
            auto metallic = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/MetallicPalette.png"), settings);
            auto roughness = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/RoughnessPalette.png"), settings);
            gfx::MaterialCache::get_material("ShipLit").set_param("albedoTex", color);
            gfx::MaterialCache::get_material("ShipLit").set_param("useAlbedoTex", true);
            gfx::MaterialCache::get_material("ShipLit").set_param("emissiveTex", emissive);
            gfx::MaterialCache::get_material("ShipLit").set_param("useEmissiveTex", true);
            gfx::MaterialCache::get_material("ShipLit").set_param("roughnessTex", roughness);
            gfx::MaterialCache::get_material("ShipLit").set_param("useRoughnessTex", true);
            gfx::MaterialCache::get_material("ShipLit").set_param("metallicTex", metallic);
            gfx::MaterialCache::get_material("ShipLit").set_param("useMetallicTex", true);
        }

        {
            gfx::MaterialCache::create_material("TargetReticle", fs::view("assets://shaders/crosshair.shs"));
            gfx::MaterialCache::get_material("TargetReticle").set_param("_texture", gfx::TextureCache::create_texture(fs::view("assets://textures/TargetReticle/TargetReticle.png")));
        }

        {
            gfx::MaterialCache::create_material("PlayerLight", fs::view("assets://shaders/light.shs"));
            gfx::MaterialCache::create_material("Light", fs::view("assets://shaders/light.shs"));
        }

        ship = createEntity("Ship");
        ship.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ gfx::MaterialCache::get_material("ShipLit"),  gfx::ModelCache::get_handle("Ship") });
        auto [playerPos, playerRot, playerScal] = ship.add_component<transform>();
        playerPos = position(0.f, 0.f, 10.f);
        ship.add_component<audio::audio_listener>();
        ship.add_component<ship_comp>();
        auto rb = ship.add_component<rigidbody>();
        rb->linearDrag = .2f;
        rb->setMass(.1f);

        auto col = ship.add_component<collider>();
        col->layer = 4;
        col->ignoreMask = 4;
        col->add_shape<SphereCollider>(math::vec3(0.f), math::vec3(1.f), 1.5f);

        //auto forceField = createEntity();
        //auto [pos, rot, scal] = forceField.add_component<transform>();
        //scal = scale(3.f);

        //forceField.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ gfx::MaterialCache::get_material("ForceField"),  gfx::ModelCache::get_handle("Bullet") });
        //ship.add_child(forceField);

        //audio::audio_source& audioSource = ship.add_component<audio::audio_source>(audio::AudioSegmentCache::getAudioSegment("BGMusic"));
        //audioSource.setLooping(true);
        ////audioSource.play();

        camera = createEntity("Camera");
        camera.add_component<transform>(position(0.f, 0.f, 0.f), rotation::lookat(position(0.f, 0.0f, 0.0f), playerPos->xyz() + position(0.f, 0.9f, -8.f)), scale());
        rigidbody& camrb = camera.add_component<rigidbody>();
        camrb.linearDrag = .2f;
        camrb.setMass(.1f);

        rendering::camera cam;
        cam.set_projection(60.f, 0.001f, 1000.f);
        camera.add_component<gfx::camera>(cam);

        for (size_type i = 0; i < 200; i++)
        {
            auto asteroid = createEntity("Asteroid" + std::to_string(i));
            auto [pos, rot, scal] = asteroid.add_component<transform>();
            scal = scale(1.f) * math::linearRand(1.f, 20.f);
            rot = rotation(math::sphericalRand(1.f));
            pos = math::ballRand(500.f);
            asteroid.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ gfx::MaterialCache::get_material("ShipLit"),  gfx::ModelCache::get_handle("Asteroid1") });
            auto col = asteroid.add_component<collider>();
            col->layer = 1;
            col->ignoreMask = 1 | 2;
            col->add_shape<SphereCollider>(math::vec3(0.f), math::vec3(1.f), 1.f);

            auto rb = asteroid.add_component<rigidbody>();
            rb->setMass(15.f);
        }
    }

    std::vector<fs::view> textures;
    {
        for (size_type i = 1; i <= 16; i++)
        {
            textures.push_back(fs::view("assets://textures/fx/explosion/frame (" + std::to_string(i) + ").png"));
        }
        auto explosionArray = gfx::TextureCache::create_texture_array("explosion", textures);
        gfx::MaterialCache::create_material("Explosion", fs::view("assets://shaders/particle.shs"));
        gfx::MaterialCache::get_material("Explosion").set_param("_texture", explosionArray);
        gfx::MaterialCache::get_material("Explosion").set_param("frameCount", 16);
        gfx::MaterialCache::get_material("Explosion").set_variant("depth_only");
        gfx::MaterialCache::get_material("Explosion").set_param("_texture", explosionArray);
        gfx::MaterialCache::get_material("Explosion").set_param("frameCount", 16);
        gfx::MaterialCache::get_material("Explosion").set_variant("default");
    }
    textures.clear();
    {
        for (size_type i = 1; i <= 16; i++)
        {
            textures.push_back(fs::view("assets://textures/fx/smoke/frame (" + std::to_string(i) + ").png"));
        }
        auto smokeArray = gfx::TextureCache::create_texture_array("smoke", textures);
        gfx::MaterialCache::create_material("Smoke", fs::view("assets://shaders/particle.shs"));
        gfx::MaterialCache::get_material("Smoke").set_param("_texture", smokeArray);
        gfx::MaterialCache::get_material("Smoke").set_param("frameCount", 16);
        gfx::MaterialCache::get_material("Smoke").set_variant("depth_only");
        gfx::MaterialCache::get_material("Smoke").set_param("_texture", smokeArray);
        gfx::MaterialCache::get_material("Smoke").set_param("frameCount", 16);
        gfx::MaterialCache::get_material("Smoke").set_variant("default");
    }

    //SpawnEnemies
    {
        for (size_type i = 0; i < 100; i++)
        {
            //spawnEnemy();
        }
    }

    //reticle = createEntity("TargetReticle");
    //reticle.add_component<transform>();
    //scale& scal = reticle.get_component<scale>();
    //scal = scale(.5f);
    //position& pos = reticle.get_component<position>();
    //pos = position(0.f, 0.f, 10.f);
    //reticle.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ gfx::MaterialCache::get_material("TargetReticle"), gfx::ModelCache::get_handle("Particle") });
    //camera.add_child(reticle);

    bindToEvent<collision, &GameSystem::onCollision>();
    timeSinceStart.start();

    createProcess<&GameSystem::fixedUpdate>("Update", 0.02f);
}

void GameSystem::fixedUpdate(lgn::time::span deltaTime)
{
    //if (!camera)
    //    return;

    direction = app::InputSystem::getMousePosition() - math::dvec2(.5);
    if (math::abs(direction.x) < .05f)
        direction.x = 0.f;
    if (math::abs(direction.y) < .05f)
        direction.y = 0.f;

    rotation& rot = camera.get_component<rotation>();
    rotation target = rot * (rotation)(math::vec3(direction.y, direction.x, math::length(direction)));
    rot = math::slerp(rot, target, static_cast<float>(deltaTime));
    rot *= math::angleAxis(rollValue * radialMovement, math::vec3::forward);

    rigidbody& rb = camera.get_component<rigidbody>();
    auto force = rot.right() * strafeValue * linearMovement;
    force += rot.up() * verticalValue * linearMovement;
    force += rot.forward() * thrustValue * linearMovement;
    rb.addForce(force);

    shootBuffer += deltaTime;
    if (ship.get_component<ship_comp>()->shooting && shootBuffer > shootInterval)
    {
        shootBuffer = 0.0f;
        shoot(ship);
    }
}

void GameSystem::onGUI(app::window& context, L_MAYBEUNUSED gfx::camera& cam, L_MAYBEUNUSED const gfx::camera::camera_input& camInput, time::span deltaTime)
{
    using namespace imgui;

    auto windowSize = context.framebufferSize();

    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));

    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiColorEditFlags_NoInputs | ImGuiWindowFlags_NoMouseInputs);

    auto* imwindow = ImGui::GetCurrentWindow();
    imwindow->FontWindowScale = 2.f;

    ImGui::Text("SCORE: %d", static_cast<int>(score + timeSinceStart.elapsed_time().seconds()));
    ImGui::End();


    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::SetNextWindowPos({ 0,10.f });
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiColorEditFlags_NoInputs | ImGuiWindowFlags_NoMouseInputs);

    imwindow = ImGui::GetCurrentWindow();
    imwindow->FontWindowScale = 2.f;

    ImGui::Text("Health: %f", static_cast<float>(ship.get_component<ship_comp>()->health));
    ImGui::End();


    static float timeBuffer = 0.f;
    static float spawninterval = 15.f;
    timeBuffer += deltaTime;

    if (timeBuffer > spawninterval && enemyCount < maxEnemies)
    {
        timeBuffer -= spawninterval;
        spawninterval *= 0.95f;
        //spawnEnemy();
    }

    //targetId = MouseHover::getHoveredEntityId();
    //if (ecs::Registry::checkEntity(targetId))
    //{
    //    if (ecs::Registry::getEntity(targetId).has_component<enemy_comp>())
    //    {
    //        target = ecs::Registry::getEntity(targetId);
    //        targeting = true;
    //    }
    //}
    //else
    //{
    //    targeting = false;
    //    position& retPos = reticle.get_component<position>();
    //    retPos = camera.get_component<position>();
    //    retPos = retPos - position(0.f, 0.f, 1.f);
    //}

    //if (target && targeting)
    //{
    //    if (target.has_component<enemy_comp>())
    //    {
    //        position& retPos = reticle.get_component<position>();
    //        retPos = target.get_component<position>();
    //    }
    //}

    ecs::filter<particle_emitter> emitterFilter;
    for (auto& ent : emitterFilter)
    {
        particle_emitter& emitter = ent.get_component<particle_emitter>();
        if (emitter.m_elapsedTime > emitter.m_targetTime)
        {
            ent.destroy();
        }
    }
}

void GameSystem::spawnEnemy()
{
    static size_type i = 0;
    auto enemy = createEntity("Enemy" + std::to_string(i++));
    enemyCount = i;
    auto [pos, rot, scal] = enemy.add_component<transform>();
    scal = scale(1.f);
    pos = math::sphericalRand(70.f);
    enemy.add_component<enemy_comp>();
    auto rb = enemy.add_component<rigidbody>();
    enemy.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ gfx::MaterialCache::get_material("ShipLit"), gfx::ModelCache::get_handle("Enemy") });
    rb->linearDrag = 1.1f;
    rb->setMass(.8f);

    auto col = enemy.add_component<collider>();
    col->layer = 2;
    col->ignoreMask = 1 | 2;
    col->add_shape<SphereCollider>(math::vec3(0.f), math::vec3(1.f), 2.5f);
}

void GameSystem::roll(player_roll& axis)
{
    rollValue = axis.value * axis.input_delta;
}
void GameSystem::strafe(player_strafe& axis)
{
    strafeValue = axis.value * axis.input_delta;
}
void GameSystem::vertical(player_vertical& axis)
{
    verticalValue = axis.value * axis.input_delta;
}
void GameSystem::thrust(player_thrust& axis)
{
    thrustValue = axis.value * axis.input_delta;
}
void GameSystem::onShoot(player_shoot& action)
{
    if (escaped)
        return;
    ecs::filter<position, rotation, scale, ship_comp> playerFilter;
    for (auto& ent : playerFilter)
    {
        if (action.pressed())
            ent.get_component<ship_comp>()->shooting = true;
        else if (action.released())
            ent.get_component<ship_comp>()->shooting = false;
    }
}
void GameSystem::stop_velocity(stop_vel& action)
{
    using namespace lgn;
    if (action.pressed())
    {
        camera.get_component<rigidbody>()->velocity = math::vec3::zero;
    }
}

void GameSystem::shoot(ecs::entity ship)
{
    using namespace lgn;
    if (ship.get_component<ship_comp>()->shooting)
    {
        static size_type i = 0;
        auto bullet = createEntity("Bullet" + std::to_string(i++));
        bullet.add_component<transform>();
        bullet.add_component<rigidbody>();
        bullet.add_component<bullet_comp>();
        bullet.add_component<gfx::light>(gfx::light::point(math::colors::yellow, 5.f, 8.f));
        bullet.add_component<audio::audio_source>(audio::AudioSegmentCache::getAudioSegment("LaserShot"));

        if (bullet.has_component<audio::audio_source>())
        {
            audio::audio_source& source = bullet.get_component<audio::audio_source>();
            source.play();
        }

        position playerPos = ship.get_component<position>();
        rotation playerRot = ship.get_component<rotation>();
        position& bulletPos = bullet.get_component<position>();
        rotation& bulletRot = bullet.get_component<rotation>();
        scale& bulletScal = bullet.get_component<scale>();
        position targetPos;
        //if (target && targeting)
        //    targetPos = target.get_component<position>();
        //else
        targetPos = (position)(playerPos.xyz() + playerRot.forward());

        bulletPos = playerPos.xyz() + playerRot.forward() * .5f;
        bulletRot = rotation::lookat(bulletPos, targetPos, playerRot.up());
        bulletScal = scale(.2f, .2f, 1.5f);

        auto model = gfx::ModelCache::get_handle("Bullet");
        auto material = gfx::MaterialCache::get_material("PlayerLight");
        material.set_param("color", math::colors::yellow);
        material.set_param("intensity", 2.f);
        bullet.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ material, model });

        rigidbody& bulletRb = bullet.get_component<rigidbody>();
        rigidbody playerRb = ship.get_component<rigidbody>();

        bulletRb.velocity = playerRb.velocity;
        bulletRb.setMass(.1f);

        auto col = bullet.add_component<collider>();
        col->layer = 4;
        col->ignoreMask = 4;
        col->add_shape<SphereCollider>();
    }
}

void GameSystem::onCollision(collision& event)
{
    //log::debug("Collision between {} and {}, with normal {}, and depth {}",
    //    event.first->name.empty() ? std::to_string(event.first->id) : event.first->name,
    //    event.second->name.empty() ? std::to_string(event.second->id) : event.second->name,
    //    event.normal.axis, event.normal.depth);

    bool playerHit = false;

    ecs::entity other{ nullptr };
    ecs::entity bullet{ nullptr };

    if (event.first.has_component<bullet_comp>())
    {
        bullet = event.first;
        other = event.second;
    }
    else if (event.second.has_component<bullet_comp>())
    {
        bullet = event.second;
        other = event.first;
    }

    if (bullet)
    {
        bullet_comp& bulletComp = bullet.get_component<bullet_comp>();

        if (other.has_component<ship_comp>())
        {
            ship_comp& playerComp = other.get_component<ship_comp>();
            //playerComp.health -= bulletComp.damge;
            playerHit = true;

            if (playerComp.health <= 0.f)
            {
                raiseEvent<events::exit>();
                log::debug("Score: {}", static_cast<int>(score + timeSinceStart.elapsed_time().seconds()));
            }
        }
        else if (other.has_component<enemy_comp>())
        {
            enemy_comp& enemyComp = other.get_component<enemy_comp>().get();
            enemyComp.health -= bulletComp.damge;

            log::debug("enemy {} health: {}", other->name, enemyComp.health);

            if (enemyComp.health <= 0.f)
            {
                spawnExplosion(other);
                other.destroy();
                score += 5;
                return;
            }
        }
    }

    position& posA = event.first.get_component<position>();
    position& posB = event.second.get_component<position>();

    auto normal = math::normalize(posA - posB);
    auto mtv = normal * std::abs(event.normal.depth);

    rigidbody& rbA = event.first.get_component<rigidbody>();
    rigidbody& rbB = event.second.get_component<rigidbody>();

    float cumulativeMass = (1.f / rbA.inverseMass) + (1.f / rbB.inverseMass);

    float massParcentageA = (1.f / rbA.inverseMass) / cumulativeMass;
    float massParcentageB = 1.f - massParcentageA;

    float energy = math::dot(rbA.velocity, -normal) + math::dot(rbB.velocity, normal);

    math::vec3 reflectVel;
    if (event.first.has_component<bullet_comp>())
    {
        posA += mtv * massParcentageB;

        rbA.addForce(normal * -massParcentageA * energy);
        reflectVel = math::normalize(posA);

    }
    else if (event.second.has_component<bullet_comp>())
    {
        posB -= mtv * massParcentageA;
        rbB.addForce(normal * massParcentageB * energy);
        reflectVel = math::normalize(posB);
    }
    else
    {
        rbA.addForce(normal * massParcentageB * energy);
        rbB.addForce(normal * -massParcentageA * energy);
    }

    if (playerHit)
    {
        position& bulletPos = bullet.get_component<position>();
        //spawnBlasterBurn(bulletPos, reflectVel);
        if (bullet)
            bullet.destroy();
    }

}

void GameSystem::initInput()
{
    using namespace legion;
    app::InputSystem::createBinding<fullscreen_action>(app::inputmap::method::F11);
    app::InputSystem::createBinding<vsync_action>(app::inputmap::method::F1);
    app::InputSystem::createBinding<tonemap_action>(app::inputmap::method::F2);
    app::InputSystem::createBinding<auto_exposure_action>(app::inputmap::method::F4);
    app::InputSystem::createBinding<exit_action>(app::inputmap::method::ESCAPE);

    app::InputSystem::createBinding<player_shoot>(app::inputmap::method::MOUSE_LEFT);
    app::InputSystem::createBinding<player_roll>(app::inputmap::method::Q, 1.f);
    app::InputSystem::createBinding<player_roll>(app::inputmap::method::E, -1.f);
    app::InputSystem::createBinding<player_thrust>(app::inputmap::method::W, 1.f);
    app::InputSystem::createBinding<player_thrust>(app::inputmap::method::S, -1.f);
    app::InputSystem::createBinding<player_strafe>(app::inputmap::method::A, -1.f);
    app::InputSystem::createBinding<player_strafe>(app::inputmap::method::D, 1.f);
    app::InputSystem::createBinding<player_vertical>(app::inputmap::method::LEFT_SHIFT, -1.f);
    app::InputSystem::createBinding<player_vertical>(app::inputmap::method::SPACE, 1.f);
    app::InputSystem::createBinding<stop_vel>(app::inputmap::method::V);

    bindToEvent<player_roll, &GameSystem::roll>();
    bindToEvent<player_strafe, &GameSystem::strafe>();
    bindToEvent<player_vertical, &GameSystem::vertical>();
    bindToEvent<player_thrust, &GameSystem::thrust>();
    bindToEvent<player_shoot, &GameSystem::onShoot>();
    bindToEvent<stop_vel, &GameSystem::stop_velocity>();

    bindToEvent<tonemap_action, &GameSystem::onTonemapSwitch>();
    bindToEvent<auto_exposure_action, &GameSystem::onAutoExposureSwitch>();
    bindToEvent<exit_action, &GameSystem::onExit>();
    bindToEvent<fullscreen_action, &GameSystem::onFullscreen>();
    bindToEvent<vsync_action, &GameSystem::onVSYNCSwap>();
}

void GameSystem::spawnExplosion(ecs::entity ent)
{
    static size_type i = 0;
    auto smokeParticles = createEntity("Smoke" + std::to_string(i));
    smokeParticles.add_component<transform>();
    particle_emitter& smokeEmitter = smokeParticles.add_component<particle_emitter>();
    smokeEmitter.set_spawn_rate(10);
    smokeEmitter.set_spawn_interval(0.2f);
    smokeEmitter.m_targetTime = 1.f;
    smokeEmitter.create_uniform<float>("minLifeTime", .4f);
    smokeEmitter.create_uniform<float>("maxLifeTime", .4f);
    smokeEmitter.create_uniform<scale>("initScale", scale(2.f));
    smokeEmitter.create_uniform<int>("frameCount", 16);
    smokeEmitter.resize(40);
    smokeEmitter.localSpace = true;
    smokeEmitter.add_policy<rendering_policy>(gfx::rendering_policy{ gfx::ModelCache::get_handle("Particle") , gfx::MaterialCache::get_material("Smoke") });
    smokeEmitter.add_policy<explosion_policy>();
    smokeEmitter.add_policy<scale_lifetime_policy>();
    smokeEmitter.add_policy<flipbook_policy>();

    auto explosionParticles = createEntity("Explosion" + std::to_string(i++));
    explosionParticles.add_component<transform>();
    particle_emitter& explosionEmitter = explosionParticles.add_component<particle_emitter>();
    explosionEmitter.set_spawn_rate(10);
    explosionEmitter.set_spawn_interval(0.02f);
    explosionEmitter.m_targetTime = 1.f;
    explosionEmitter.create_uniform<float>("minLifeTime", .4f);
    explosionEmitter.create_uniform<float>("maxLifeTime", .4f);
    explosionEmitter.create_uniform<scale>("initScale", scale(2.f));
    explosionEmitter.create_uniform<int>("frameCount", 16);
    explosionEmitter.resize(40);
    explosionEmitter.localSpace = true;
    explosionEmitter.add_policy<rendering_policy>(gfx::rendering_policy{ gfx::ModelCache::get_handle("Particle") , gfx::MaterialCache::get_material("Explosion") });
    explosionEmitter.add_policy<explosion_policy>();
    explosionEmitter.add_policy<scale_lifetime_policy>();
    explosionEmitter.add_policy<flipbook_policy>();

    position pos = ent.get_component<position>();
    smokeParticles.get_component<position>() = pos;
    explosionParticles.get_component<position>() = pos;
}

void GameSystem::spawnBlasterBurn(math::vec3 hitPoint, math::vec3 reflectVel)
{
    static size_type i = 0;
    particle_emitter& blastParticleEmitter = ship.add_component<particle_emitter>();
    blastParticleEmitter.set_spawn_rate(10);
    blastParticleEmitter.set_spawn_interval(0.2f);
    blastParticleEmitter.m_targetTime = 10.f;
    blastParticleEmitter.create_uniform<float>("minLifeTime", .2f);
    blastParticleEmitter.create_uniform<float>("maxLifeTime", 1.f);
    blastParticleEmitter.create_uniform<scale>("initScale", scale(.1f, .1f, 1.f));
    blastParticleEmitter.create_uniform<float>("scaleFactor", 1.f);
    blastParticleEmitter.resize(40);
    blastParticleEmitter.localSpace = true;
    auto material = gfx::MaterialCache::get_material("Light");
    material.set_param("color", math::colors::red);
    material.set_param("intensity", 2.f);
    blastParticleEmitter.add_policy<rendering_policy>(gfx::rendering_policy{ gfx::ModelCache::get_handle("Bullet") , material });
    blastParticleEmitter.add_policy<scale_lifetime_policy>();
    transform trans = ship.get_component<transform>();
    blastParticleEmitter.add_policy<fountain_policy>(fountain_policy{ (trans.to_world_matrix() * math::vec4(hitPoint,1.f)).xyz(), reflectVel });
}

void GameSystem::onAutoExposureSwitch(auto_exposure_action& event)
{
    using namespace legion;
    if (event.released())
    {
        static float defaultExposure = gfx::Tonemapping::getExposure();
        bool enabled = !gfx::Tonemapping::autoExposureEnabled();
        gfx::Tonemapping::enableAutoExposure(enabled);
        if (!enabled)
            gfx::Tonemapping::setExposure(defaultExposure);

        log::debug("Auto exposure {}", enabled ? "enabled" : "disabled");
    }
}

void GameSystem::onTonemapSwitch(tonemap_action& event)
{
    using namespace legion;
    if (event.released())
    {
        static size_type type = static_cast<size_type>(gfx::tonemapping_type::aces);
        type = (type + 1) % (static_cast<size_type>(gfx::tonemapping_type::unreal3) + 1);

        auto typeEnum = static_cast<gfx::tonemapping_type>(type);

        std::string algorithmName;
        switch (typeEnum)
        {
        case gfx::tonemapping_type::aces:
            algorithmName = "aces";
            break;
        case gfx::tonemapping_type::reinhard:
            algorithmName = "reinhard";
            break;
        case gfx::tonemapping_type::reinhard_jodie:
            algorithmName = "reinhard_jodie";
            break;
        case gfx::tonemapping_type::legion:
            algorithmName = "legion";
            break;
        case gfx::tonemapping_type::unreal3:
            algorithmName = "unreal3";
            break;
        default:
            algorithmName = "legion";
            break;
        }
        log::debug("Set tonemapping algorithm to {}", algorithmName);

        gfx::Tonemapping::setAlgorithm(typeEnum);
    }
}

void GameSystem::onExit(exit_action& action)
{
    using namespace lgn;
    if (GuiTestSystem::isEditingText)
        return;

    if (action.released())
        raiseEvent<events::exit>();
}

void GameSystem::onFullscreen(fullscreen_action& action)
{
    using namespace lgn;
    if (GuiTestSystem::isEditingText)
        return;

    if (action.released())
    {
        app::WindowSystem::requestFullscreenToggle(ecs::world_entity_id, math::ivec2(100, 100), math::ivec2(1360, 768));
    }
}

void GameSystem::onVSYNCSwap(vsync_action& action)
{
    using namespace lgn;
    if (GuiTestSystem::isEditingText)
        return;

    if (action.released())
    {
        app::window& window = ecs::world.get_component<app::window>();
        window.setSwapInterval(window.swapInterval() ? 0 : 1);
        log::debug("set swap interval to {}", window.swapInterval());
    }
}
