#include "autogen_prototype_particle_uniform_base.hpp"
#include "../../core/particles/particleuniform.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::particle_uniform_base>(const legion::core::particle_uniform_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::particle_uniform_base>();
        prot.typeName = "legion::core::particle_uniform_base";
        return prot;
    }
}
