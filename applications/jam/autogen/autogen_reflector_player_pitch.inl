#include "autogen_reflector_player_pitch.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_pitch>(::player_pitch& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_pitch>();
        refl.typeName = "player_pitch";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_pitch>(const ::player_pitch& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_pitch>();
        refl.typeName = "player_pitch";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
