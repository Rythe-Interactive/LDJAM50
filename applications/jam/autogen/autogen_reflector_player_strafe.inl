#include "autogen_reflector_player_strafe.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_strafe>(::player_strafe& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_strafe>();
        refl.typeName = "player_strafe";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_strafe>(const ::player_strafe& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_strafe>();
        refl.typeName = "player_strafe";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
