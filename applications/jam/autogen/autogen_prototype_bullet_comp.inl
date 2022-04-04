#include "autogen_prototype_bullet_comp.hpp"
#include "../../jam/broadphase/../data/../components/bulletcomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::bullet_comp>(const ::bullet_comp& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::bullet_comp>();
        prot.typeName = "bullet_comp";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("damge", member_value("damge", primitive_value{typeHash(obj.damge), std::any(obj.damge)}));
        prot.members.emplace("lifetime", member_value("lifetime", primitive_value{typeHash(obj.lifetime), std::any(obj.lifetime)}));
        prot.members.emplace("age", member_value("age", primitive_value{typeHash(obj.age), std::any(obj.age)}));
        return prot;
    }
}
