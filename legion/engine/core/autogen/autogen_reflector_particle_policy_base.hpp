#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct particle_policy_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::particle_policy_base>(legion::core::particle_policy_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::particle_policy_base>(const legion::core::particle_policy_base& obj);
}
