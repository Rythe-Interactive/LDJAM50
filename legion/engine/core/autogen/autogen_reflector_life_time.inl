#include "autogen_reflector_life_time.hpp"
#include "../../core/particles/particlesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::life_time>(legion::core::life_time& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::life_time>();
        refl.typeName = "legion::core::life_time";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::life_time>(const legion::core::life_time& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::life_time>();
        refl.typeName = "legion::core::life_time";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
