#define LEGION_ENTRY
#include "engine_include.hpp"

#include "module/gamemodule.hpp"


void LEGION_CCONV reportModules(legion::Engine* engine)
{
    using namespace legion;

    engine->reportModule<app::ApplicationModule>();
    engine->reportModule<gfx::RenderingModule>();
    engine->reportModule<audio::AudioModule>();
    engine->reportModule<physics::PhysicsModule>();
    engine->reportModule<GameModule>();
}
