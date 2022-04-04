#include "autogen_prototype_player_pitch.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_pitch>(const ::player_pitch& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_pitch>();
        prot.typeName = "player_pitch";
        return prot;
    }
}
