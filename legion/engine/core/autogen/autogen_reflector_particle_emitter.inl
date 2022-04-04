#include "autogen_reflector_particle_emitter.hpp"
#include "../../core/particles/particleemitter.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::particle_emitter>(legion::core::particle_emitter& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::particle_emitter>();
        refl.typeName = "legion::core::particle_emitter";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::particle_emitter>(const legion::core::particle_emitter& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::particle_emitter>();
        refl.typeName = "legion::core::particle_emitter";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
