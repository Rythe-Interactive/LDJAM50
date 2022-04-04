#include "autogen_reflector_collision_pair.hpp"
#include "../../jam/data/collision_pair.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::collision_pair>(::collision_pair& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::collision_pair>();
        refl.typeName = "collision_pair";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::collision_pair>(const ::collision_pair& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::collision_pair>();
        refl.typeName = "collision_pair";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
