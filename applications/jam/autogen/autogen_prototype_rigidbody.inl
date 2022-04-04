#include "autogen_prototype_rigidbody.hpp"
#include "../../jam/module/../systems/../components/rigidbody.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::rigidbody>(const ::rigidbody& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::rigidbody>();
        prot.typeName = "rigidbody";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("inverseMass", member_value("inverseMass", primitive_value{typeHash(obj.inverseMass), std::any(obj.inverseMass)}));
        prot.members.emplace("linearDrag", member_value("linearDrag", primitive_value{typeHash(obj.linearDrag), std::any(obj.linearDrag)}));
        prot.members.emplace("angularDrag", member_value("angularDrag", primitive_value{typeHash(obj.angularDrag), std::any(obj.angularDrag)}));
        prot.members.emplace("restitution", member_value("restitution", primitive_value{typeHash(obj.restitution), std::any(obj.restitution)}));
        prot.members.emplace("friction", member_value("friction", primitive_value{typeHash(obj.friction), std::any(obj.friction)}));
        return prot;
    }
}
