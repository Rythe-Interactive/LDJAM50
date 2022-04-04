#include "autogen_prototype_player_yaw.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_yaw>(const ::player_yaw& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_yaw>();
        prot.typeName = "player_yaw";
        return prot;
    }
}
