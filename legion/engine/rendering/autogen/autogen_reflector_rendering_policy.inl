#include "autogen_reflector_rendering_policy.hpp"
#include "../../rendering/data/particlepolicies/renderpolicy.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::rendering_policy>(legion::rendering::rendering_policy& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::rendering_policy>();
        refl.typeName = "legion::rendering::rendering_policy";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::rendering_policy>(const legion::rendering::rendering_policy& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::rendering_policy>();
        refl.typeName = "legion::rendering::rendering_policy";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
