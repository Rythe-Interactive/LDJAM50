#include "autogen_prototype_player_comp.hpp"
#include "../../jam/components/playercomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::ship_comp>(const ::ship_comp& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::ship_comp>();
        prot.typeName = "ship_comp";
        return prot;
    }
}
