#pragma once
#include "../engine_include.hpp"

#include "../renderstages/mousehover.hpp"
#include "gui_test.hpp"

struct tonemap_action : public lgn::app::input_action<tonemap_action> {};
struct switch_skybox_action : public lgn::app::input_action<switch_skybox_action> {};
struct auto_exposure_action : public lgn::app::input_action<auto_exposure_action> {};
struct reload_shaders_action : public lgn::app::input_action<reload_shaders_action> {};

class ExampleSystem final : public legion::System<ExampleSystem>
{
    lgn::size_type frames = 0;
    lgn::time64 totalTime = 0;
    lgn::time::stopwatch<lgn::time64> timer;
    std::array<lgn::time64, 18000> times;

public:
    void setup();

    void shutdown()
    {
        lgn::log::debug("ExampleSystem shutdown");
    }

    void onShaderReload(reload_shaders_action& event);

    void onAutoExposureSwitch(auto_exposure_action& event);
    
    void onTonemapSwitch(tonemap_action& event);

    void onSkyboxSwitch(switch_skybox_action& event);

    void onExit(L_MAYBEUNUSED lgn::events::exit& event);

    void update(legion::time::span deltaTime);
};
