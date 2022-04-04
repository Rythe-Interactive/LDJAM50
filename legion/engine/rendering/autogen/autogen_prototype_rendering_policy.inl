#include "autogen_prototype_rendering_policy.hpp"
#include "../../rendering/data/particlepolicies/renderpolicy.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::rendering_policy>(const legion::rendering::rendering_policy& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::rendering_policy>();
        prot.typeName = "legion::rendering::rendering_policy";
        return prot;
    }
}
