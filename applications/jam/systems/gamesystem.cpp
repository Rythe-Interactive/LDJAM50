#include <chrono>

#include "../systems/gamesystem.hpp"
#include "../components/components.hpp"
#include "../defaults/defaultpolicies.hpp"

#include <rendering/data/particlepolicies/renderpolicy.hpp>
#include <rendering/data/particlepolicies/flipbookpolicy.hpp>


void GameSystem::setup()
{
    using namespace legion;
    log::filter(log::severity_debug);
    log::debug("GameSystem setup");

    auto* pipeline = dynamic_cast<gfx::DefaultPipeline*>(gfx::Renderer::getMainPipeline());
    if (pipeline)
        pipeline->attachStage<MouseHover>();

    app::window& window = ecs::world.get_component<app::window>();
    window.enableCursor(false);
    window.show();
    app::context_guard guard(window);

    auto groundplane = createEntity("Ground Plane");
    auto groundmat = rendering::MaterialCache::create_material("floor", "assets://shaders/groundplane.shs"_view);
    groundmat.set_param("floorTile", rendering::TextureCache::create_texture("floorTile", "engine://resources/default/tile.png"_view));
    groundmat.set_variant("depth_only");
    groundmat.set_param("floorTile", rendering::TextureCache::create_texture("floorTile", "engine://resources/default/tile.png"_view));
    groundmat.set_variant("default");
    groundplane.add_component(gfx::mesh_renderer{ groundmat, rendering::ModelCache::create_model("floor", "assets://models/plane.obj"_view) });
    groundplane.add_component<transform>();

    auto skyboxMat = rendering::MaterialCache::create_material("skybox", "assets://shaders/skybox.shs"_view);
    skyboxMat.set_param(SV_SKYBOX, TextureCache::create_texture("planet atmo", fs::view("assets://textures/HDRI/planetatmo.png"),
        texture_import_settings
        {
            texture_type::two_dimensional, true, channel_format::eight_bit, texture_format::rgba_hdr,
            texture_components::rgba, true, true, 0, texture_mipmap::linear_mipmap_linear, texture_mipmap::linear,
            texture_wrap::edge_clamp, texture_wrap::repeat, texture_wrap::edge_clamp
        }));
    ecs::world.add_component(gfx::skybox_renderer{ skyboxMat });

    gfx::ModelCache::create_model("Bullet", fs::view("assets://models/sphere.obj"));
    auto material = gfx::MaterialCache::create_material("BulletMat", fs::view("assets://shaders/texture.shs"));
    material.set_param("_texture", gfx::TextureCache::create_texture("Default", fs::view("engine://resources/default/albedo")));

    initInput();

    //Serialization Test
    srl::SerializerRegistry::registerSerializer<example_comp>();
    srl::SerializerRegistry::registerSerializer<ecs::entity>();
    srl::SerializerRegistry::registerSerializer<position>();
    srl::SerializerRegistry::registerSerializer<rotation>();
    srl::SerializerRegistry::registerSerializer<velocity>();
    srl::SerializerRegistry::registerSerializer<scale>();
    srl::SerializerRegistry::registerSerializer<assets::import_settings<mesh>>();
    srl::SerializerRegistry::registerSerializer<sub_mesh>();
    srl::SerializerRegistry::registerSerializer<mesh>();
    srl::SerializerRegistry::registerSerializer<assets::asset<mesh>>();
    srl::SerializerRegistry::registerSerializer<material_data>();
    srl::SerializerRegistry::registerSerializer<mesh_filter>();

    //srl::load<srl::bson, ecs::entity>(fs::view("assets://scenes/scene1.bson"), "scene");
    auto player = createEntity("Player");
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
    player.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ material, model });
    auto [pos, rot, scal] = player.add_component<transform>();
    player.add_component<player_comp>();
    auto rb = player.add_component<rigidbody>();
    rb->linearDrag = 1.1f;
    rb->setMass(.1f);

    auto camera_ent = createEntity("Camera");
    camera_ent.add_component<transform>(position(0.f, 2.f, -8.f), rotation::lookat(math::vec3(0.f, 2.f, -8.f), pos->xyz() + math::vec3(0.f, 1.f, 0.f)), scale());
    camera_ent.add_component<audio::audio_listener>();
    rendering::camera cam;
    cam.set_projection(60.f, 0.001f, 1000.f);
    camera_ent.add_component<gfx::camera>(cam);
    player.add_child(camera_ent);

    model = gfx::ModelCache::create_model("Enemy", fs::view("assets://models/ship/JamStealth.glb"));
    for (size_type i = 0; i < 200; i++)
    {
        auto enemy = createEntity();
        auto [pos, rot, scal] = enemy.add_component<transform>();
        scal = scale(.3f);
        pos = math::ballRand(10.f);
        enemy.add_component<enemy_comp>();
        auto rb = enemy.add_component<physics::rigidbody>();
        enemy.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ material, model });
        rb->linearDrag = 1.1f;
        rb->setMass(.8f);
    }

    for (size_type i = 0; i < 50; i++)
    {
        auto asteroid = createEntity();
        auto [pos, rot, scal] = asteroid.add_component<transform>();
        scal = scale(1.f) * math::linearRand(1.f, 2.f);
        pos = math::ballRand(25.f);
        model = gfx::ModelCache::create_model("Asteroid1", fs::view("assets://models/asteroid/JamAsteroid1.glb"));
        asteroid.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ material, model });
    }
}

void GameSystem::update(legion::time::span deltaTime)
{
    using namespace legion;
    if (escaped)
        mouseOver();
}

void GameSystem::pitch(player_pitch& axis)
{
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

        if (angle > -0.001f)
            angle = -0.001f;
        if (angle < -(math::pi<float>() - 0.001f))
            angle = -(math::pi<float>() - 0.001f);

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
    using namespace lgn;
    ecs::filter<position, rotation, scale, player_comp> playerFilter;
    for (auto& ent : playerFilter)
    {
        if (action.pressed())
        {
            auto bullet = createEntity();

            bullet.add_component<gfx::light>(gfx::light::point(math::colors::white, 5.f, 16.f));
            auto& e_pos = ent.get_component<position>().get();
            auto& e_rot = ent.get_component<rotation>().get();
            auto [b_pos, b_rot, b_scal] = bullet.add_component<transform>();
            b_pos = e_pos.xyz() + e_rot.forward() * 2.f;
            b_scal = scale(.1f);

            auto model = gfx::ModelCache::get_handle("Bullet");
            auto material = gfx::MaterialCache::get_material("ShipLit");
            //material.set_param("_texture", gfx::TextureCache::create_texture("Default", fs::view("engine://resources/default/albedo")));
            bullet.add_component<gfx::mesh_renderer>(gfx::mesh_renderer{ material, model });

            bullet.add_component<bullet_comp>();
            auto shootDir = ent.get_component<rotation>()->forward();
            auto p_vel = ent.get_component<rigidbody>()->velocity;
            auto& b_rb = bullet.add_component<rigidbody>().get();
            b_rb.velocity = p_vel;
            b_rb.addForce(shootDir * 1500.f);
        }
    }
}

void GameSystem::mouseOver()
{
    using namespace lgn;
    auto hoveredEntityId = MouseHover::getHoveredEntityId();
    if (hoveredEntityId != invalid_id)
    {
        auto ent = ecs::Registry::getEntity(hoveredEntityId);

        if (ent != GuiTestSystem::selected && ent.has_component<transform>())
        {
            transform transf = ent.get_component<transform>();

            math::mat4 worldMat = transf.to_world_matrix();
            math::vec3 min = math::vec3(-0.5f, -0.5f, -0.5f);
            math::vec3 max = math::vec3(0.5f, 0.5f, 0.5f);

            std::pair<math::vec3, math::vec3> edges[] = {
                std::make_pair(min, math::vec3(min.x, min.y, max.z)),
                std::make_pair(math::vec3(min.x, min.y, max.z), math::vec3(max.x, min.y, max.z)),
                std::make_pair(math::vec3(max.x, min.y, max.z), math::vec3(max.x, min.y, min.z)),
                std::make_pair(math::vec3(max.x, min.y, min.z), min),

                std::make_pair(max, math::vec3(max.x, max.y, min.z)),
                std::make_pair(math::vec3(max.x, max.y, min.z), math::vec3(min.x, max.y, min.z)),
                std::make_pair(math::vec3(min.x, max.y, min.z), math::vec3(min.x, max.y, max.z)),
                std::make_pair(math::vec3(min.x, max.y, max.z), max),

                std::make_pair(min, math::vec3(min.x, max.y, min.z)),
                std::make_pair(math::vec3(min.x, min.y, max.z), math::vec3(min.x, max.y, max.z)),
                std::make_pair(math::vec3(max.x, min.y, max.z), math::vec3(max.x, max.y, max.z)),
                std::make_pair(math::vec3(max.x, min.y, min.z), math::vec3(max.x, max.y, min.z))
            };

            for (auto& edge : edges)
                debug::drawLine((worldMat * math::vec4(edge.first.x, edge.first.y, edge.first.z, 1.f)).xyz(), (worldMat * math::vec4(edge.second.x, edge.second.y, edge.second.z, 1.f)).xyz(), math::colors::orange, 10.f);
        }
    }

    if (GuiTestSystem::selected != invalid_id)
    {
        if (GuiTestSystem::selected.has_component<transform>())
        {
            transform transf = GuiTestSystem::selected.get_component<transform>();

            math::mat4 worldMat = transf.to_world_matrix();
            math::vec3 min = math::vec3(-0.5f, -0.5f, -0.5f);
            math::vec3 max = math::vec3(0.5f, 0.5f, 0.5f);

            std::pair<math::vec3, math::vec3> edges[] = {
                std::make_pair(min, math::vec3(min.x, min.y, max.z)),
                std::make_pair(math::vec3(min.x, min.y, max.z), math::vec3(max.x, min.y, max.z)),
                std::make_pair(math::vec3(max.x, min.y, max.z), math::vec3(max.x, min.y, min.z)),
                std::make_pair(math::vec3(max.x, min.y, min.z), min),

                std::make_pair(max, math::vec3(max.x, max.y, min.z)),
                std::make_pair(math::vec3(max.x, max.y, min.z), math::vec3(min.x, max.y, min.z)),
                std::make_pair(math::vec3(min.x, max.y, min.z), math::vec3(min.x, max.y, max.z)),
                std::make_pair(math::vec3(min.x, max.y, max.z), max),

                std::make_pair(min, math::vec3(min.x, max.y, min.z)),
                std::make_pair(math::vec3(min.x, min.y, max.z), math::vec3(min.x, max.y, max.z)),
                std::make_pair(math::vec3(max.x, min.y, max.z), math::vec3(max.x, max.y, max.z)),
                std::make_pair(math::vec3(max.x, min.y, min.z), math::vec3(max.x, max.y, min.z))
            };

            for (auto& edge : edges)
                debug::drawLine((worldMat * math::vec4(edge.first.x, edge.first.y, edge.first.z, 1.f)).xyz(), (worldMat * math::vec4(edge.second.x, edge.second.y, edge.second.z, 1.f)).xyz(), math::colors::green, 10.f);
        }
    }
}

void GameSystem::initInput()
{
    using namespace legion;
    app::InputSystem::createBinding<restart_action>(app::inputmap::method::F10);
    app::InputSystem::createBinding<fullscreen_action>(app::inputmap::method::F11);
    app::InputSystem::createBinding<escape_cursor_action>(app::inputmap::method::F9);
    app::InputSystem::createBinding<vsync_action>(app::inputmap::method::F1);
    app::InputSystem::createBinding<exit_action>(app::inputmap::method::ESCAPE);
    app::InputSystem::createBinding<tonemap_action>(app::inputmap::method::F2);
    app::InputSystem::createBinding<switch_skybox_action>(app::inputmap::method::F3);
    app::InputSystem::createBinding<auto_exposure_action>(app::inputmap::method::F4);
    app::InputSystem::createBinding<reload_shaders_action>(app::inputmap::method::F5);

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
    bindToEvent<switch_skybox_action, &GameSystem::onSkyboxSwitch>();
    bindToEvent<auto_exposure_action, &GameSystem::onAutoExposureSwitch>();
    bindToEvent<reload_shaders_action, &GameSystem::onShaderReload>();
    bindToEvent<exit_action, &GameSystem::onExit>();
    bindToEvent<restart_action, &GameSystem::onRestart>();
    bindToEvent<fullscreen_action, &GameSystem::onFullscreen>();
    bindToEvent<escape_cursor_action, &GameSystem::onEscapeCursor>();
    bindToEvent<vsync_action, &GameSystem::onVSYNCSwap>();
}

void GameSystem::onGetCamera(lgn::time::span)
{
    using namespace lgn;
    static ecs::filter<gfx::camera> query{};

    if (query.size())
    {
        camera = query[0];
    }
}

void GameSystem::onShaderReload(reload_shaders_action& event)
{
    using namespace legion;
    if (event.released())
    {
        auto targetWin = ecs::world.get_component<app::window>();
        if (!targetWin)
            return;

        app::window& win = targetWin.get();

        if (!app::WindowSystem::windowStillExists(win.handle))
            return;

        app::context_guard guard(win);
        if (guard.contextIsValid())
        {
            gfx::ShaderCache::clear_checked_paths();
            gfx::ShaderCache::reload_shaders();

            auto [lock, materials] = gfx::MaterialCache::get_all_materials();
            async::readonly_guard materialsGuard(lock);
            for (auto& [id, mat] : materials)
                mat.reload();
        }
    }
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

void GameSystem::onSkyboxSwitch(switch_skybox_action& event)
{
    using namespace legion;
    using namespace rendering;

    ecs::filter<skybox_renderer> filter;
    if (filter.empty())
        return;

    if (event.released())
    {
        static size_type idx = 0;
        static texture_handle textures[4] = {};
        static bool initialized = false;

        if (!initialized)
        {
            auto targetWin = ecs::world.get_component<app::window>();
            if (!targetWin)
                return;

            app::window& win = targetWin.get();

            if (!app::WindowSystem::windowStillExists(win.handle))
                return;

            app::context_guard guard(win);
            if (guard.contextIsValid())
            {
                auto importSettings = texture_import_settings{
                        texture_type::two_dimensional, true, channel_format::eight_bit, texture_format::rgba_hdr,
                        texture_components::rgba, true, true, 0, texture_mipmap::linear_mipmap_linear, texture_mipmap::linear,
                        texture_wrap::edge_clamp,texture_wrap::repeat, texture_wrap::edge_clamp };

                textures[0] = TextureCache::create_texture("morning islands", fs::view("assets://textures/HDRI/morning_islands.jpg"), importSettings);
                textures[1] = TextureCache::create_texture("earth", fs::view("assets://textures/HDRI/earth.png"), importSettings);
                textures[2] = TextureCache::create_texture("park", fs::view("assets://textures/HDRI/park.jpg"), importSettings);
                textures[3] = TextureCache::create_texture("atmosphere", fs::view("assets://textures/HDRI/planetatmo.png"), importSettings);
                initialized = true;
            }
            else
                return;
        }

        idx = (idx + 1) % 4;
        auto skyboxRenderer = filter[0].get_component<skybox_renderer>();
        skyboxRenderer->material.set_param(SV_SKYBOX, textures[idx]);

        log::debug("Set skybox to {}", textures[idx].get_texture().name);
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

void GameSystem::onRestart(restart_action& action)
{
    using namespace lgn;
    if (GuiTestSystem::isEditingText)
        return;

    if (action.released())
        this_engine::restart();
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

void GameSystem::onEscapeCursor(escape_cursor_action& action)
{
    using namespace lgn;
    if (GuiTestSystem::isEditingText)
        return;

    if (action.released() && (action.mods & app::inputmap::modifier_keys::CTRL))
    {
        if (!escaped)
        {
            app::window& window = ecs::world.get_component<app::window>();
            escaped = true;
            window.enableCursor(true);
        }
        else
        {
            app::window& window = ecs::world.get_component<app::window>();
            escaped = false;
            window.enableCursor(false);
        }
    }

    GuiTestSystem::CaptureKeyboard(!escaped);
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
