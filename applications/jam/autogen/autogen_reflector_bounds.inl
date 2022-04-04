#include "autogen_reflector_bounds.hpp"
#include "../../jam/defaults/defaultpolicies.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::bounds>(legion::core::bounds& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::bounds>();
        refl.typeName = "legion::core::bounds";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::bounds>(const legion::core::bounds& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::bounds>();
        refl.typeName = "legion::core::bounds";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
