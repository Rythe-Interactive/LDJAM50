#include "autogen_reflector_player_roll.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_roll>(::player_roll& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_roll>();
        refl.typeName = "player_roll";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_roll>(const ::player_roll& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_roll>();
        refl.typeName = "player_roll";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
