#include "autogen_prototype_life_time.hpp"
#include "../../core/particles/particlesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::life_time>(const legion::core::life_time& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::life_time>();
        prot.typeName = "legion::core::life_time";
        return prot;
    }
}
