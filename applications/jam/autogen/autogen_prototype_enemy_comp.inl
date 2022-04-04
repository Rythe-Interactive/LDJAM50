#include "autogen_prototype_enemy_comp.hpp"
#include "../../jam/components/enemycomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::enemy_comp>(const ::enemy_comp& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::enemy_comp>();
        prot.typeName = "enemy_comp";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("target", member_value("target", primitive_value{typeHash(obj.target), std::any(obj.target)}));
        prot.members.emplace("health", member_value("health", primitive_value{typeHash(obj.health), std::any(obj.health)}));
        prot.members.emplace("visionRadius", member_value("visionRadius", primitive_value{typeHash(obj.visionRadius), std::any(obj.visionRadius)}));
        prot.members.emplace("playerHuntRadius", member_value("playerHuntRadius", primitive_value{typeHash(obj.playerHuntRadius), std::any(obj.playerHuntRadius)}));
        prot.members.emplace("playerSeperationRadius", member_value("playerSeperationRadius", primitive_value{typeHash(obj.playerSeperationRadius), std::any(obj.playerSeperationRadius)}));
        prot.members.emplace("seperationRadius", member_value("seperationRadius", primitive_value{typeHash(obj.seperationRadius), std::any(obj.seperationRadius)}));
        prot.members.emplace("speed", member_value("speed", primitive_value{typeHash(obj.speed), std::any(obj.speed)}));
        return prot;
    }
}
