#include "autogen_reflector_player_thrust.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_thrust>(::player_thrust& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_thrust>();
        refl.typeName = "player_thrust";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_thrust>(const ::player_thrust& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_thrust>();
        refl.typeName = "player_thrust";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
