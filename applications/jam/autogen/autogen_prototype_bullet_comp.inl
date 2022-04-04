#include "autogen_prototype_bullet_comp.hpp"
#include "../../jam/components/bulletcomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::bullet_comp>(const ::bullet_comp& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::bullet_comp>();
        prot.typeName = "bullet_comp";
        return prot;
    }
}
