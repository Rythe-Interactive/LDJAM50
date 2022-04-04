#include "autogen_prototype_collision_pair.hpp"
#include "../../jam/data/collision_pair.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::collision_pair>(const ::collision_pair& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::collision_pair>();
        prot.typeName = "collision_pair";
        return prot;
    }
}
