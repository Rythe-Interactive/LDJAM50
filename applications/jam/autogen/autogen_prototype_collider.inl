#include "autogen_prototype_collider.hpp"
#include "../../jam/module/../systems/../components/collider.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::collider>(const ::collider& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::collider>();
        prot.typeName = "collider";
        return prot;
    }
}
