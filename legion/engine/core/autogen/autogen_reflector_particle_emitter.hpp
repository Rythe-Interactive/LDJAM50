#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct particle_emitter;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::particle_emitter>(legion::core::particle_emitter& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::particle_emitter>(const legion::core::particle_emitter& obj);
}
