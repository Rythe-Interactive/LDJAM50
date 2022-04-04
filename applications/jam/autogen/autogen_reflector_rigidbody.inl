#include "autogen_reflector_rigidbody.hpp"
#include "../../jam/module/../systems/../components/rigidbody.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::rigidbody>(::rigidbody& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::rigidbody>();
        refl.typeName = "rigidbody";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("inverseMass", member_reference("inverseMass", primitive_reference{typeHash(obj.inverseMass), &obj.inverseMass}));
        refl.members.emplace("linearDrag", member_reference("linearDrag", primitive_reference{typeHash(obj.linearDrag), &obj.linearDrag}));
        refl.members.emplace("angularDrag", member_reference("angularDrag", primitive_reference{typeHash(obj.angularDrag), &obj.angularDrag}));
        refl.members.emplace("restitution", member_reference("restitution", primitive_reference{typeHash(obj.restitution), &obj.restitution}));
        refl.members.emplace("friction", member_reference("friction", primitive_reference{typeHash(obj.friction), &obj.friction}));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::rigidbody>(const ::rigidbody& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::rigidbody>();
        refl.typeName = "rigidbody";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("inverseMass", member_reference("inverseMass", primitive_reference{typeHash(obj.inverseMass), &obj.inverseMass}));
        refl.members.emplace("linearDrag", member_reference("linearDrag", primitive_reference{typeHash(obj.linearDrag), &obj.linearDrag}));
        refl.members.emplace("angularDrag", member_reference("angularDrag", primitive_reference{typeHash(obj.angularDrag), &obj.angularDrag}));
        refl.members.emplace("restitution", member_reference("restitution", primitive_reference{typeHash(obj.restitution), &obj.restitution}));
        refl.members.emplace("friction", member_reference("friction", primitive_reference{typeHash(obj.friction), &obj.friction}));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
