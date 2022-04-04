#include "autogen_prototype_flipbook_policy.hpp"
#include "../../rendering/data/particlepolicies/flipbookpolicy.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::flipbook_policy>(const legion::rendering::flipbook_policy& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::flipbook_policy>();
        prot.typeName = "legion::rendering::flipbook_policy";
        return prot;
    }
}
