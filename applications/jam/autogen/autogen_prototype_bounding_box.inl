#include "autogen_prototype_bounding_box.hpp"
#include "../../jam/data/bounding_box.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::bounding_box>(const ::bounding_box& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::bounding_box>();
        prot.typeName = "bounding_box";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        return prot;
    }
}
