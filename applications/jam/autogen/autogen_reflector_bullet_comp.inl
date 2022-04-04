#include "autogen_reflector_bullet_comp.hpp"
#include "../../jam/components/bulletcomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::bullet_comp>(::bullet_comp& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::bullet_comp>();
        refl.typeName = "bullet_comp";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::bullet_comp>(const ::bullet_comp& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::bullet_comp>();
        refl.typeName = "bullet_comp";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
