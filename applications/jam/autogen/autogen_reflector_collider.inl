#include "autogen_reflector_collider.hpp"
#include "../../jam/module/../systems/../components/collider.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::collider>(::collider& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::collider>();
        refl.typeName = "collider";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::collider>(const ::collider& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::collider>();
        refl.typeName = "collider";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
