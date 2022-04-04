#include "autogen_prototype_player_roll.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_roll>(const ::player_roll& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_roll>();
        prot.typeName = "player_roll";
        return prot;
    }
}
