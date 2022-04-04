#include "autogen_prototype_simplex.hpp"
#include "../../jam/data/simplex.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::simplex>(const ::simplex& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::simplex>();
        prot.typeName = "simplex";
        return prot;
    }
}
