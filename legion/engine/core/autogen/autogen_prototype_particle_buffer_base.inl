#include "autogen_prototype_particle_buffer_base.hpp"
#include "../../core/particles/particlebuffer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::particle_buffer_base>(const legion::core::particle_buffer_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::particle_buffer_base>();
        prot.typeName = "legion::core::particle_buffer_base";
        return prot;
    }
}
