#pragma once
#include "../engine_include.hpp"

#include "../renderstages/mousehover.hpp"
#include "gui_test.hpp"

#include "../data/collision_data.hpp"

struct [[reflectable]] example_comp
{
    int value = 1;
};

struct tonemap_action : public lgn::app::input_action<tonemap_action> {};
struct switch_skybox_action : public lgn::app::input_action<switch_skybox_action> {};
struct auto_exposure_action : public lgn::app::input_action<auto_exposure_action> {};
struct reload_shaders_action : public lgn::app::input_action<reload_shaders_action> {};

struct player_pitch : public lgn::app::input_axis<player_pitch> {};
struct player_roll : public lgn::app::input_axis<player_roll> {};
struct player_yaw : public lgn::app::input_axis<player_yaw> {};
struct player_thrust : public lgn::app::input_axis<player_thrust> {};
struct player_shoot : public lgn::app::input_action<player_shoot> {};
struct player_strafe : public lgn::app::input_axis<player_strafe> {};
struct player_vertical : public lgn::app::input_axis<player_vertical> {};

struct exit_action : public lgn::app::input_action<exit_action> {};
struct restart_action : public lgn::app::input_action<restart_action> {};

struct fullscreen_action : public lgn::app::input_action<fullscreen_action> {};
struct escape_cursor_action : public lgn::app::input_action<escape_cursor_action> {};
struct vsync_action : public lgn::app::input_action<vsync_action> {};

class GameSystem final : public legion::System<GameSystem>
{
public:
    lgn::ecs::entity camera;
    bool escaped = false;
    float linearMovement = 75.f;
    float radialMovement = 500.f;

    time::timer timeSinceStart;

    void setup();

    void onGUI(L_MAYBEUNUSED app::window& context, gfx::camera& cam, const gfx::camera::camera_input& camInput, L_MAYBEUNUSED time::span deltaTime);

    void pitch(player_pitch& axis);
    void roll(player_roll& axis);
    void yaw(player_yaw& axis);
    void strafe(player_strafe& axis);
    void vertical(player_vertical& axis);
    void thrust(player_thrust& axis);
    void shoot(player_shoot& action);

    void onCollision(collision& event);
    void initInput();

    void onAutoExposureSwitch(auto_exposure_action& event);
    void onTonemapSwitch(tonemap_action& event);
    void onExit(exit_action& action);
    void onFullscreen(fullscreen_action& action);
    void onVSYNCSwap(vsync_action& action);
};
