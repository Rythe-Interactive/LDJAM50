#include "autogen_prototype_enemy_comp.hpp"
#include "../../jam/components/enemycomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::enemy_comp>(const ::enemy_comp& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::enemy_comp>();
        prot.typeName = "enemy_comp";
        return prot;
    }
}
