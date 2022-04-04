#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct particle_buffer_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::particle_buffer_base>(legion::core::particle_buffer_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::particle_buffer_base>(const legion::core::particle_buffer_base& obj);
}
