#include "autogen_prototype_player_vertical.hpp"
#include "../../jam/systems/gamesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_vertical>(const ::player_vertical& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_vertical>();
        prot.typeName = "player_vertical";
        return prot;
    }
}
