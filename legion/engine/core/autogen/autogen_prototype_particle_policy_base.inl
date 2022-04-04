#include "autogen_prototype_particle_policy_base.hpp"
#include "../../core/particles/particlepolicy.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::particle_policy_base>(const legion::core::particle_policy_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::particle_policy_base>();
        prot.typeName = "legion::core::particle_policy_base";
        return prot;
    }
}
