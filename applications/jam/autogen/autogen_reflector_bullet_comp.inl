#include "autogen_reflector_bullet_comp.hpp"
#include "../../jam/broadphase/../data/../components/bulletcomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::bullet_comp>(::bullet_comp& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::bullet_comp>();
        refl.typeName = "bullet_comp";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("damge", member_reference("damge", primitive_reference{typeHash(obj.damge), &obj.damge}));
        refl.members.emplace("lifetime", member_reference("lifetime", primitive_reference{typeHash(obj.lifetime), &obj.lifetime}));
        refl.members.emplace("age", member_reference("age", primitive_reference{typeHash(obj.age), &obj.age}));
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
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("damge", member_reference("damge", primitive_reference{typeHash(obj.damge), &obj.damge}));
        refl.members.emplace("lifetime", member_reference("lifetime", primitive_reference{typeHash(obj.lifetime), &obj.lifetime}));
        refl.members.emplace("age", member_reference("age", primitive_reference{typeHash(obj.age), &obj.age}));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
