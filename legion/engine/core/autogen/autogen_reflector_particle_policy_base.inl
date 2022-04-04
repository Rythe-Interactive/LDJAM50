#include "autogen_reflector_particle_policy_base.hpp"
#include "../../core/particles/particlepolicy.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::particle_policy_base>(legion::core::particle_policy_base& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::particle_policy_base>();
        refl.typeName = "legion::core::particle_policy_base";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::particle_policy_base>(const legion::core::particle_policy_base& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::particle_policy_base>();
        refl.typeName = "legion::core::particle_policy_base";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
