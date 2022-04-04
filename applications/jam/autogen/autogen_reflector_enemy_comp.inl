#include "autogen_reflector_enemy_comp.hpp"
#include "../../jam/components/enemycomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::enemy_comp>(::enemy_comp& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::enemy_comp>();
        refl.typeName = "enemy_comp";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::enemy_comp>(const ::enemy_comp& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::enemy_comp>();
        refl.typeName = "enemy_comp";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
