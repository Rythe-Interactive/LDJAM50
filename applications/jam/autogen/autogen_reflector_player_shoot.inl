#include "autogen_reflector_player_shoot.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_shoot>(::player_shoot& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_shoot>();
        refl.typeName = "player_shoot";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_shoot>(const ::player_shoot& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_shoot>();
        refl.typeName = "player_shoot";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
