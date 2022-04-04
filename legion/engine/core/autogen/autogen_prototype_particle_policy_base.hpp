#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct particle_policy_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::particle_policy_base>(const legion::core::particle_policy_base& obj);
}
