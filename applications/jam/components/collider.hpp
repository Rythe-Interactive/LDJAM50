#pragma once
#include "../engine_include.hpp"

#include "../colliders/collisionshape.hpp"

struct collider
{
    bounding_box bounds;

    uint8 layer = 0;
    uint8 ignoreMask = 0;

    std::vector<std::unique_ptr<CollisionShape>> shapes;

    collider() = default;
    MOVE_FUNCS(collider);
    ~collider() = default;

    template<typename ShapeType, std::enable_if_t<std::is_base_of_v<CollisionShape, ShapeType>, bool> = true, typename... Args>
    void add_shape(Args&&... args)
    {
        if (shapes.empty())
        {
            bounds.min = math::vec3(std::numeric_limits<float>::max());
            bounds.max = math::vec3(std::numeric_limits<float>::lowest());
        }

        shapes.push_back(std::make_unique<ShapeType>(std::forward<Args>(args)...));
        bounds.expand(shapes.at(shapes.size() - 1)->bounds);
    }
};

struct [[no_reflect]] collidable : public ecs::archetype<position, rotation, scale, collider>
{
    using base = ecs::archetype<position, rotation, scale, collider>;
    using base::archetype;

    L_NODISCARD math::mat4 from_world_matrix()
    {
        return math::inverse(to_world_matrix());
    }

    L_NODISCARD math::mat4 to_world_matrix()
    {
        if (owner->parent)
        {
            transform parentTrans = owner->parent.get_component<transform>();
            if (parentTrans)
                return parentTrans.to_world_matrix() * to_parent_matrix();
        }
        return to_parent_matrix();
    }

    L_NODISCARD math::mat4 from_parent_matrix()
    {
        return math::inverse(to_parent_matrix());
    }

    L_NODISCARD math::mat4 to_parent_matrix()
    {
        auto [position, rotation, scale, _] = values();
        return math::compose(scale, rotation, position);
    }
};
