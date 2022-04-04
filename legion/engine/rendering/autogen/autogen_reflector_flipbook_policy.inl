#include "autogen_reflector_flipbook_policy.hpp"
#include "../../rendering/data/particlepolicies/flipbookpolicy.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::flipbook_policy>(legion::rendering::flipbook_policy& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::flipbook_policy>();
        refl.typeName = "legion::rendering::flipbook_policy";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::flipbook_policy>(const legion::rendering::flipbook_policy& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::flipbook_policy>();
        refl.typeName = "legion::rendering::flipbook_policy";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
