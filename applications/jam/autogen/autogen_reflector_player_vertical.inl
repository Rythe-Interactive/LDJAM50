#include "autogen_reflector_player_vertical.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_vertical>(::player_vertical& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_vertical>();
        refl.typeName = "player_vertical";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_vertical>(const ::player_vertical& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_vertical>();
        refl.typeName = "player_vertical";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
