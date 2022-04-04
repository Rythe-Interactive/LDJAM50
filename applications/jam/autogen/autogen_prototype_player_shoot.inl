#include "autogen_prototype_player_shoot.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_shoot>(const ::player_shoot& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_shoot>();
        prot.typeName = "player_shoot";
        return prot;
    }
}
