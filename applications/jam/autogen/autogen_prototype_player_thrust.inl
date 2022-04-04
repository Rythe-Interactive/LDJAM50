#include "autogen_prototype_player_thrust.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_thrust>(const ::player_thrust& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_thrust>();
        prot.typeName = "player_thrust";
        return prot;
    }
}
