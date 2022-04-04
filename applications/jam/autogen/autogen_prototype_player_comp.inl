#include "autogen_prototype_player_comp.hpp"
#include "../../jam/components/playercomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_comp>(const ::player_comp& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_comp>();
        prot.typeName = "player_comp";
        return prot;
    }
}
