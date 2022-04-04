#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct particle_uniform_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::particle_uniform_base>(legion::core::particle_uniform_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::particle_uniform_base>(const legion::core::particle_uniform_base& obj);
}
