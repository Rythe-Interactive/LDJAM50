#include "autogen_reflector_player_roll.hpp"
#include "../components/components.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::camera_follow>(::camera_follow& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::camera_follow>();
        refl.typeName = "camera_follow";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::camera_follow>(const ::camera_follow& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::camera_follow>();
        refl.typeName = "camera_follow";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
