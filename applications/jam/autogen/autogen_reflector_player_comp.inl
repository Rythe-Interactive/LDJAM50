#include "autogen_reflector_player_comp.hpp"
#include "../../jam/components/playercomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::ship_comp>(::ship_comp& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::ship_comp>();
        refl.typeName = "ship_comp";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::ship_comp>(const ::ship_comp& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::ship_comp>();
        refl.typeName = "ship_comp";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
