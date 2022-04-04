#include "autogen_prototype_particle_emitter.hpp"
#include "../../core/particles/particleemitter.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::particle_emitter>(const legion::core::particle_emitter& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::particle_emitter>();
        prot.typeName = "legion::core::particle_emitter";
        return prot;
    }
}
