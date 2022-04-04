#include "autogen_reflector_bounding_box.hpp"
#include "../../jam/data/bounding_box.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::bounding_box>(::bounding_box& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::bounding_box>();
        refl.typeName = "bounding_box";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::bounding_box>(const ::bounding_box& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::bounding_box>();
        refl.typeName = "bounding_box";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
