#pragma once
#include "../engine_include.hpp"

#include "../data/collision_data.hpp"

#include "../debug_utils.hpp"

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
        pairs.clear();
        size_type collidableCount = m_colliderQuery.size();
        pairs.reserve(collidableCount * collidableCount);

        for (size_type i = 0; i < collidableCount; i++)
        {
            auto& first = m_colliderQuery[i];
            collidable firstCollidable = first.get_component<collidable>();

            math::vec3 firstPosition;
            math::quat firstRotation;
            math::vec3 firstScale;
            math::decompose(firstCollidable.to_world_matrix(), firstScale, firstRotation, firstPosition);

            auto& firstCollider = firstCollidable.get<collider>();

            for (size_type j = i + 1; j < collidableCount; j++)
            {
                auto& second = m_colliderQuery[j];
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

class SpacialHash final : public BroadPhase
{
    constexpr static float cellSize = 25.f;

    struct [[no_reflect]] collidable_data
    {
        ecs::entity ent;
        math::vec3 position;
        math::quat rotation;
        math::vec3 scale;
        pointer<collider> coll;
    };

    using pool = std::vector<collidable_data>;

    class SpacialHasher
    {
        std::vector<id_type> m_cache;

    public:
        const std::vector<id_type>& getHashes(const bounding_box& bounds)
        {
            static_assert(sizeof(id_type) == sizeof(diff_type));
            m_cache.clear();

            auto min = math::vec<3, diff_type>((bounds.min / cellSize));

            if (min.x < 1)
                min.x--;

            if (min.y < 1)
                min.y--;

            if (min.z < 1)
                min.z--;

            auto max = math::vec<3, diff_type>((bounds.max / cellSize));

            if (max.x < 0)
                max.x--;

            if (max.y < 0)
                max.y--;

            if (max.z < 0)
                max.z--;

            for (auto x = min.x; x <= max.x; x++)
                for (auto y = min.y; y <= max.y; y++)
                    for (auto z = min.z; z <= max.z; z++)
                        m_cache.push_back(combine_hash(*reinterpret_cast<id_type*>(&x), combine_hash(*reinterpret_cast<id_type*>(&y), *reinterpret_cast<id_type*>(&z))));

            return m_cache;
        }

    } m_hasher;

    std::unordered_map<id_type, pool> m_pools;

    std::unordered_set<id_type> m_knownPairs;

    bool checkNan(const math::vec3& vec)
    {
        return (std::isnan(vec.x) || std::isnan(vec.y) || std::isnan(vec.z) ||
            !std::isfinite(vec.x) || !std::isfinite(vec.y) || !std::isfinite(vec.z));
    }

    bool checkNan(const math::quat& rot)
    {
        return (std::isnan(rot.x) || std::isnan(rot.y) || std::isnan(rot.z) || std::isnan(rot.w) ||
            !std::isfinite(rot.x) || !std::isfinite(rot.y) || !std::isfinite(rot.z) || !std::isfinite(rot.w));
    }

    id_type getPairHash(ecs::entity a, ecs::entity b)
    {
        return combine_hash(a->id, b->id);
    }

    virtual void sortPairs(std::vector<collision_pair>& pairs)
    {
        pairs.clear();
        m_knownPairs.clear();

        size_type collidableCount = m_colliderQuery.size();
        pairs.reserve(collidableCount * collidableCount);

        for (auto& [hash, pool] : m_pools)
            pool.clear();

        for (size_type i = 0; i < collidableCount; i++)
        {
            collidable_data data;
            data.ent = m_colliderQuery[i];
            collidable collidableArch = data.ent.get_component<collidable>();

            math::mat4 worldMat = collidableArch.to_world_matrix();
            math::decompose(worldMat, data.scale, data.rotation, data.position);

            if (checkNan(data.position) || checkNan(data.rotation) || checkNan(data.scale))
                continue;

            data.coll = { &collidableArch.get<collider>() };

            std::array<math::vec3, 8> verts;
            data.coll->bounds.get_verts(verts);

            for (auto& vert : verts)
                vert = (worldMat * math::vec4(vert, 1.f)).xyz();

            bounding_box worldBounds;
            worldBounds.min = math::vec3(std::numeric_limits<float>::max());
            worldBounds.max = math::vec3(std::numeric_limits<float>::lowest());

            for (auto& vert : verts)
                worldBounds.expand(vert);

            for (auto& hash : m_hasher.getHashes(worldBounds))
                m_pools[hash].push_back(data);
        }

        for (auto& [hash, pool] : m_pools)
        {
            size_type poolSize = pool.size();
            for (size_type i = 0; i < poolSize; i++)
            {
                auto& first = pool[i];
                for (size_type j = i + 1; j < poolSize; j++)
                {
                    auto& second = pool[j];

                    if (first.ent == second.ent)
                        continue;

                    if (second.coll->ignoreMask & first.coll->layer)
                        continue;

                    if (first.coll->ignoreMask & second.coll->layer)
                        continue;

                    id_type pairHash = getPairHash(first.ent, second.ent);
                    if (!m_knownPairs.count(pairHash))
                    {
                        log::debug("pair {} {}", first.ent->name, second.ent->name);

                        m_knownPairs.emplace(pairHash);
                        pairs.emplace_back(first.ent, first.position, first.rotation, first.scale, std::ref(*first.coll), second.ent, second.position, second.rotation, second.scale, std::ref(*second.coll));
                    }
                }
            }
        }
    }
};
