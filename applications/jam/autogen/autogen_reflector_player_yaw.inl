#include "autogen_reflector_player_yaw.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_yaw>(::player_yaw& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_yaw>();
        refl.typeName = "player_yaw";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_yaw>(const ::player_yaw& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_yaw>();
        refl.typeName = "player_yaw";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
