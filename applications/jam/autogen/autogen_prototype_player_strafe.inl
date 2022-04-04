#include "autogen_prototype_player_strafe.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_strafe>(const ::player_strafe& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_strafe>();
        prot.typeName = "player_strafe";
        return prot;
    }
}
