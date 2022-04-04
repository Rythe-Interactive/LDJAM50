#pragma once
#include <core/types/prototype.hpp>
struct rigidbody;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::rigidbody>(const ::rigidbody& obj);
}
