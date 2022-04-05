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
    }

    ImGuiStage::addGuiRender<GameSystem, &GameSystem::onGUI>(this);

    app::window& window = ecs::world.get_component<app::window>();
    window.enableCursor(false);
    window.show();
    app::context_guard guard(window);

    auto skyboxMat = rendering::MaterialCache::create_material("skybox", "assets://shaders/skybox.shs"_view);
    skyboxMat.set_param(SV_SKYBOX, TextureCache::create_texture("planet atmo", fs::view("assets://textures/HDRI/planetatmo7.png"),
        texture_import_settings
        {
            texture_type::two_dimensional, true, channel_format::eight_bit, texture_format::rgba_hdr,
            texture_components::rgba, true, true, 0, texture_mipmap::linear_mipmap_linear, texture_mipmap::linear,
            texture_wrap::edge_clamp, texture_wrap::repeat, texture_wrap::edge_clamp
        }));
    ecs::world.add_component(gfx::skybox_renderer{ skyboxMat });

    gfx::ModelCache::create_model("Particle", fs::view("assets://models/billboard.glb"));
    gfx::ModelCache::create_model("Bullet", fs::view("assets://models/sphere.obj"));
    auto material = gfx::MaterialCache::create_material("BulletMat", fs::view("assets://shaders/texture.shs"));
    material.set_param("_texture", gfx::TextureCache::create_texture("Default", fs::view("engine://resources/default/albedo")));

    audio::AudioSegmentCache::createAudioSegment("Explosion", fs::view("assets://audio/fx/Explosion2.wav"));
    audio::AudioSegmentCache::createAudioSegment("LaserShot", fs::view("assets://audio/fx/Laser_Shoot.wav"));
    audio::AudioSegmentCache::createAudioSegment("BGMusic", fs::view("assets://audio/background.mp3"));

    initInput();

    material = gfx::MaterialCache::create_material("PlayerLight", fs::view("assets://shaders/light.shs"));
    material = gfx::MaterialCache::create_material("Light", fs::view("assets://shaders/light.shs"));
    auto model = gfx::ModelCache::create_model("Ship", fs::view("assets://models/ship/JamShip.glb"));
    material = gfx::MaterialCache::create_material("ShipLit", fs::view("engine://shaders/default_lit.shs"));
    texture_import_settings settings = gfx::default_texture_settings;
    settings.mag = gfx::texture_mipmap::nearest;
    auto color = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/ColorPalette.png"), settings);
    auto emissive = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/EmissivePallete.png"), settings);
    auto metallic = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/MetallicPalette.png"), settings);
    auto roughness = gfx::TextureCache::create_texture(fs::view("assets://textures/ship/RoughnessPalette.png"), settings);
    material.set_param("albedoTex", color);
    material.set_param("useAlbedoTex", true);
    material.set_param("emissiveTex", emissive);
    material.set_param("useEmissiveTex", true);
    material.set_param("roughnessTex", roughness);
    material.set_param("useRoughnessTex", true);
    material.set_param("metallicTex", metallic);
    material.set_param("useMetallicTex", true);


    std::vector<fs::view> textures;
    {
        for (size_type i = 1; i <= 16; i++)
        {
            textures.push_back(fs::view("assets://textures/fx/explosion/frame (" + std::to_string(i) + ").png"));
        }
        auto explosionArray = gfx::TextureCache::create_texture_array("explosion", textures);
        auto material = gfx::MaterialCache::create_material("Explosion", fs::view("assets://shaders/particle.shs"));
        material.set_param("_texture", explosionArray);
        material.set_variant("depth_only");
        material.set_param("_texture", explosionArray);
        material.set_variant("default");
    }
    textures.clear();
    {
        for (size_type i = 1; i <= 16; i++)
        {
            textures.push_back(fs::view("assets://textures/fx/smoke/frame (" + std::to_string(i) + ").png"));
        }
        auto smokeArray = gfx::TextureCache::create_texture_array("smoke", textures);
        auto material = gfx::MaterialCache::create_material("Smoke", fs::view("assets://shaders/particle.shs"));
        material.set_param("_texture", smokeArray);
        material.set_variant("depth_only");
        material.set_param("_texture", smokeArray);
        material.set_variant("default");
    }

    auto smokeParticles = createEntity("Particle Emitter");
    smokeParticles.add_component<transform>();
    auto emitter = smokeParticles.add_component<particle_emitter>();
    emitter->set_spawn_rate(1);
    emitter->set_spawn_interval(0.2f);
    emitter->create_uniform<float>("minLifeTime", 1.f);
    emitter->create_uniform<float>("maxLifeTime", 1.f);
    emitter->create_uniform<float>("scaleFactor", 2.f);
    emitter->create_uniform<uint>("frameCount", 16);
    emitter->resize(100);
    emitter->localSpace = true;
    emitter->add_policy<rendering_policy>(gfx::rendering_policy{ gfx::ModelCache::get_handle("Particle") , gfx::MaterialCache::get_material("Explosion") });
    emitter->add_policy<explosion_policy>();
    emitter->add_policy<scale_lifetime_policy>();
    emitter->add_policy<flipbook_policy>();

    //Create Player
    {
        auto player = createEntity("Player");
        player.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ material, model });
        auto [pos, rot, scal] = player.add_component<transform>();
        player.add_component<audio::audio_listener>();
        player.add_component<player_comp>();
        auto rb = player.add_component<rigidbody>();
        rb->linearDrag = .8f;
        rb->setMass(.1f);

        auto camera_ent = createEntity("Camera");
        camera_ent.add_component<transform>(position(0.f, 2.f, -8.f), rotation::lookat(math::vec3(0.f, 2.f, -8.f), pos->xyz() + math::vec3(0.f, 1.f, 0.f)), scale());
        rendering::camera cam;
        cam.set_projection(60.f, 0.001f, 1000.f);
        camera_ent.add_component<gfx::camera>(cam);
        player.add_child(camera_ent);
        auto col = player.add_component<collider>();
        col->add_shape<SphereCollider>(math::vec3(0.f), math::vec3(1.f), 1.5f);
    }

    //SpawnEnemies
    {
        model = gfx::ModelCache::create_model("Enemy", fs::view("assets://models/ship/JamEnemy.glb"));
        for (size_type i = 0; i < 300; i++)
        {
            auto enemy = createEntity();
            auto [pos, rot, scal] = enemy.add_component<transform>();
            scal = scale(.3f);
            pos = math::sphericalRand(70.f);
            enemy.add_component<enemy_comp>();
            auto rb = enemy.add_component<rigidbody>();
            enemy.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ material, model });
            rb->linearDrag = 1.1f;
            rb->setMass(.8f);
            auto col = enemy.add_component<collider>();
            col->add_shape<SphereCollider>(math::vec3(0.f), math::vec3(1.f), 2.5f);
        }
    }

    for (size_type i = 0; i < 20; i++)
    {
        auto asteroid = createEntity("Asteroid" + std::to_string(i));
        auto [pos, rot, scal] = asteroid.add_component<transform>();
        scal = scale(1.f) * math::linearRand(1.f, 2.f);
        rot = rotation(math::sphericalRand(1.f));
        pos = math::ballRand(100.f);
        model = gfx::ModelCache::create_model("Asteroid1", fs::view("assets://models/asteroid/JamAsteroid1.glb"));
        asteroid.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ material, model });
        auto col = asteroid.add_component<collider>();
        col->add_shape<SphereCollider>(math::vec3(0.f), math::vec3(1.f), 1.f);

        auto rb = asteroid.add_component<rigidbody>();
        rb->setMass(15.f);

    }

    bindToEvent<collision, &GameSystem::onCollision>();
    timeSinceStart.start();
}

void GameSystem::onGUI(L_MAYBEUNUSED app::window& context, gfx::camera& cam, const gfx::camera::camera_input& camInput, L_MAYBEUNUSED time::span deltaTime)
{
    using namespace imgui;

    auto windowSize = context.framebufferSize();

    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));

    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiColorEditFlags_NoInputs | ImGuiWindowFlags_NoMouseInputs);

    auto* imwindow = ImGui::GetCurrentWindow();

    imwindow->FontWindowScale = 2.f;

    ImGui::Text("SCORE: %d", static_cast<int>(timeSinceStart.elapsed_time().seconds()));

    //ImGui::SameLine();
    //std::string hi = "HIGH: " + std::to_string(highscore);
    //ImVec2 textSize = ImGui::CalcTextSize(hi.c_str());
    //ImGuiStyle& style = ImGui::GetStyle();
    //ImGui::SetCursorPosX(width - textSize.x - style.ItemSpacing.x);
    //ImGui::Text(hi.c_str());
    ImGui::End();

}

void GameSystem::pitch(player_pitch& axis)
{
    if (escaped)
        return;

    using namespace lgn::core;
    ecs::filter<position, rotation, scale, player_comp> playerFilter;
    for (auto& ent : playerFilter)
    {
        rotation& rot = ent.get_component<rotation>();
        math::mat3 rotMat = math::toMat3(rot);
        math::vec3 right = rotMat * math::vec3::right;
        math::vec3 fwd = math::normalize(math::cross(right, math::vec3::up));
        math::vec3 up = rotMat * math::vec3::up;
        float angle = math::orientedAngle(fwd, up, right);

        angle += axis.value * axis.input_delta * radialMovement;

        up = math::mat3(math::axisAngleMatrix(right, angle)) * fwd;
        fwd = math::cross(right, up);
        rot = (rotation)math::conjugate(math::toQuat(math::lookAt(math::vec3::zero, fwd, up)));
    }
}
void GameSystem::roll(player_roll& axis)
{
    using namespace lgn::core;
    ecs::filter<position, rotation, scale, player_comp> playerFilter;
    for (auto& ent : playerFilter)
    {
        rotation& rot = ent.get_component<rotation>();
        rot *= math::angleAxis(axis.value * axis.input_delta, math::vec3::forward);
    }
}
void GameSystem::yaw(player_yaw& axis)
{
    if (escaped)
        return;

    using namespace lgn::core;
    ecs::filter<position, rotation, scale, player_comp> playerFilter;
    for (auto& ent : playerFilter)
    {
        rotation& rot = ent.get_component<rotation>();
        rot *= math::angleAxis(axis.value * axis.input_delta * radialMovement, math::vec3::up);
    }
}
void GameSystem::strafe(player_strafe& axis)
{
    using namespace lgn;
    ecs::filter<position, rotation, scale, player_comp> playerFilter;
    for (auto& ent : playerFilter)
    {
        rotation& rot = ent.get_component<rotation>();
        rigidbody& rb = ent.get_component<rigidbody>();
        auto force = rot.right() * axis.value * axis.input_delta * linearMovement;
        rb.addForce(force);
    }
}
void GameSystem::vertical(player_vertical& axis)
{
    using namespace lgn;
    ecs::filter<position, rotation, scale, player_comp> playerFilter;
    for (auto& ent : playerFilter)
    {
        rotation& rot = ent.get_component<rotation>();
        rigidbody& rb = ent.get_component<rigidbody>();
        auto force = rot.up() * axis.value * axis.input_delta * linearMovement;
        rb.addForce(force);
    }
}
void GameSystem::thrust(player_thrust& axis)
{
    using namespace lgn;
    ecs::filter<position, rotation, scale, player_comp> playerFilter;
    for (auto& ent : playerFilter)
    {
        rotation& rot = ent.get_component<rotation>();
        rigidbody& rb = ent.get_component<rigidbody>();
        auto force = rot.forward() * axis.value * axis.input_delta * linearMovement;
        rb.addForce(force);
    }
}
void GameSystem::shoot(player_shoot& action)
{
    if (escaped)
        return;

    using namespace lgn;
    ecs::filter<position, rotation, scale, player_comp> playerFilter;
    for (auto& ent : playerFilter)
    {
        if (action.pressed())
        {
            auto bullet = createEntity("Bullet");
            auto& light = bullet.add_component<gfx::light>(gfx::light::point(math::colors::yellow, 5.f, 8.f)).get();
            auto& e_pos = ent.get_component<position>().get();
            auto& e_rot = ent.get_component<rotation>().get();
            auto [b_pos, b_rot, b_scal] = bullet.add_component<transform>();
            b_pos = e_pos.xyz() + e_rot.forward() * .5f;
            b_rot = e_rot;
            b_scal = scale(.1f, .1f, 1.f);

            auto model = gfx::ModelCache::get_handle("Bullet");
            auto material = gfx::MaterialCache::get_material("PlayerLight");
            material.set_param("color", math::colors::yellow);
            material.set_param("intensity", 2.f);
            bullet.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ material, model });
            auto source = bullet.add_component<audio::audio_source>(audio::AudioSegmentCache::getAudioSegment("LaserShot"));
            source->play();
            bullet.add_component<bullet_comp>();
            auto shootDir = ent.get_component<rotation>()->forward();
            auto p_vel = ent.get_component<rigidbody>()->velocity;
            auto& b_rb = bullet.add_component<rigidbody>().get();
            b_rb.velocity = p_vel;
            b_rb.setMass(.1f);

            auto col = bullet.add_component<collider>();
            col->add_shape<SphereCollider>();
        }
    }
}

void GameSystem::onCollision(collision& event)
{
    log::debug("Collision between {} and {}, with normal {}, and depth {}",
        event.first->name.empty() ? std::to_string(event.first->id) : event.first->name,
        event.second->name.empty() ? std::to_string(event.second->id) : event.second->name,
        event.normal.axis, event.normal.depth);

    ecs::entity other;
    ecs::entity bullet;

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


        if (other.has_component<player_comp>())
        {
            return;
        }
        else if (other.has_component<enemy_comp>())
        {
            enemy_comp& enemyComp = other.get_component<enemy_comp>();
            enemyComp.health -= bulletComp.damge;

            log::debug("enemy {} health: {}", other->name, enemyComp.health);

            if (enemyComp.health <= 0.f)
                other.destroy();
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

    posA += mtv * massParcentageB;
    posB -= mtv * massParcentageA;

    float energy = math::dot(rbA.velocity, -normal) + math::dot(rbB.velocity, normal);

    rbA.addForce(normal * massParcentageB * energy);
    rbB.addForce(normal * -massParcentageA * energy);

    //if (bullet)
    //    bullet.destroy();
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
    app::InputSystem::createBinding<player_pitch>(app::inputmap::method::MOUSE_Y, 1.f);
    app::InputSystem::createBinding<player_pitch>(app::inputmap::method::MOUSE_Y, -1.f);
    app::InputSystem::createBinding<player_yaw>(app::inputmap::method::MOUSE_X, 1.f);
    app::InputSystem::createBinding<player_yaw>(app::inputmap::method::MOUSE_X, -1.f);
    app::InputSystem::createBinding<player_roll>(app::inputmap::method::Q, 1.f);
    app::InputSystem::createBinding<player_roll>(app::inputmap::method::E, -1.f);
    app::InputSystem::createBinding<player_thrust>(app::inputmap::method::W, 1.f);
    app::InputSystem::createBinding<player_thrust>(app::inputmap::method::S, -1.f);
    app::InputSystem::createBinding<player_strafe>(app::inputmap::method::A, -1.f);
    app::InputSystem::createBinding<player_strafe>(app::inputmap::method::D, 1.f);
    app::InputSystem::createBinding<player_vertical>(app::inputmap::method::LEFT_SHIFT, -1.f);
    app::InputSystem::createBinding<player_vertical>(app::inputmap::method::SPACE, 1.f);


    bindToEvent<player_pitch, &GameSystem::pitch>();
    bindToEvent<player_roll, &GameSystem::roll>();
    bindToEvent<player_yaw, &GameSystem::yaw>();
    bindToEvent<player_strafe, &GameSystem::strafe>();
    bindToEvent<player_vertical, &GameSystem::vertical>();
    bindToEvent<player_thrust, &GameSystem::thrust>();
    bindToEvent<player_shoot, &GameSystem::shoot>();

    bindToEvent<tonemap_action, &GameSystem::onTonemapSwitch>();
    bindToEvent<auto_exposure_action, &GameSystem::onAutoExposureSwitch>();
    bindToEvent<exit_action, &GameSystem::onExit>();
    bindToEvent<fullscreen_action, &GameSystem::onFullscreen>();
    bindToEvent<vsync_action, &GameSystem::onVSYNCSwap>();
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
