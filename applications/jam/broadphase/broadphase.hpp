#pragma once
#include "../engine_include.hpp"

#include "../data/collision_pair.hpp"

using namespace lgn;

class BroadPhase
{
protected:
    static ecs::filter<position, rotation, scale, collider> m_colliderQuery;

public:
    virtual void sortPairs(std::vector<collision_pair>& pairs) LEGION_PURE;

    virtual ~BroadPhase() = default;
};

class BruteForce final : public BroadPhase
{
    virtual void sortPairs(std::vector<collision_pair>& pairs)
    {
        pairs.reserve(m_colliderQuery.size() * m_colliderQuery.size());

        for (auto first : m_colliderQuery)
        {
            collidable firstCollidable = first.get_component<collidable>();

            math::vec3 firstPosition;
            math::quat firstRotation;
            math::vec3 firstScale;
            math::decompose(firstCollidable.to_world_matrix(), firstScale, firstRotation, firstPosition);

            auto& firstCollider = firstCollidable.get<collider>();

            for (auto second : m_colliderQuery)
                if (first != second)
                {
                    collidable secondCollidable = second.get_component<collidable>();
                    math::vec3 secondPosition;
                    math::quat secondRotation;
                    math::vec3 secondScale;
                    math::decompose(secondCollidable.to_world_matrix(), secondScale, secondRotation, secondPosition);
                    pairs.emplace_back(first, firstPosition, firstRotation, firstScale, std::ref(firstCollider), second, secondPosition, secondRotation, secondScale, std::ref(secondCollidable.get<collider>()));
                }
        }
    }
};
