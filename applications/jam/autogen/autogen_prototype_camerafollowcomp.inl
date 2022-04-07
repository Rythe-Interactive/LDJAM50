#include "autogen_prototype_camerafollowcomp.hpp"
#include "../components/components.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::camera_follow>(const ::camera_follow& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::camera_follow>();
        prot.typeName = "::camera_follow";
        return prot;
    }
}
