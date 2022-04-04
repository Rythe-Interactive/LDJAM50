#include "autogen_prototype_bounds.hpp"
#include "../../jam/module/../systems/../defaults/defaultpolicies.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::bounds>(const legion::core::bounds& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::bounds>();
        prot.typeName = "legion::core::bounds";
        return prot;
    }
}
