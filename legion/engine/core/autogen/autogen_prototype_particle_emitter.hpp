#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct particle_emitter;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::particle_emitter>(const legion::core::particle_emitter& obj);
}
