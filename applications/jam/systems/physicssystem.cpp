#include "physicssystem.hpp"
#include "../data/simplex.hpp"

std::unique_ptr<BroadPhase> PhysicsSystem::m_boardPhase(nullptr);

void PhysicsSystem::setup()
{
    if (!m_boardPhase)
        m_boardPhase = std::make_unique<SpacialHash>();

    createProcess<&PhysicsSystem::fixedUpdate>("Update", 0.02f);
}

void PhysicsSystem::fixedUpdate(time::time_span<fast_time> deltaTime)
{
    checkCollisions(deltaTime);

    integrateRigidbodies(deltaTime);
}

void PhysicsSystem::checkCollisions(float deltaTime)
{
    static std::vector<collision_pair> pairs;
    m_boardPhase->sortPairs(pairs);

    for (auto& pair : pairs)
    {
        collision data;
        if (checkCollision(pair, data))
        {
            raiseEvent<collision>(data);
        }
    }
}

constexpr static float epsilon = 0.001f;


static math::vec3 safeNormalize(const math::vec3& vec)
{
    auto length2 = math::length2(vec);
    if (length2 > epsilon)
        return vec * (1.f / ::std::sqrtf(length2));
    return vec;
}


static void getFaceNormals(const std::vector<math::vec3>& polytope, const std::vector<size_type>& faces, std::vector<collision_normal>& normals, size_type& minTri)
{
    minTri = 0;
    float minDist = std::numeric_limits<float>::max();

    for (size_type i = 0; i < faces.size(); i += 3)
    {
        auto& v0 = polytope[faces[i]];
        auto& v1 = polytope[faces[i + 1]];
        auto& v2 = polytope[faces[i + 2]];

        math::vec3 normal = safeNormalize(math::cross(v1 - v0, v2 - v0));
        float distance = math::dot(normal, v0);

        if (distance < 0.f)
        {
            normal *= -1.f;
            distance *= -1.f;
        }

        normals.push_back(collision_normal{ normal, distance });

        if (distance < minDist)
        {
            minTri = i / 3;
            minDist = distance;
        }
    }
}

void addIfUniqueEdge(std::vector<math::vec<2, size_type>>& edges, const std::vector<size_t>& faces, size_t a, size_t b)
{
    auto reverse = std::find(edges.begin(), edges.end(), math::vec<2, size_type>{faces[b], faces[a]});

    if (reverse != edges.end())
        edges.erase(reverse);
    else
        edges.emplace_back(faces[a], faces[b]);
}

static math::vec3 calculateSupportPoint(
    collider& firstCollider, const math::vec3& firstCenter, const math::mat4& firstTransf, const math::mat3& firstInvRot,
    collider& secondCollider, const math::vec3& secondCenter, const math::mat4& secondTransf, const math::mat3& secondInvRot,
    const math::vec3& direction)
{
    math::vec3 firstSupport = firstCenter;
    float furthestDist2 = 0.f;
    math::vec3 dir = firstInvRot * direction;

    for (auto& shape : firstCollider.shapes)
    {
        auto shapeSupport = shape->getSupportPoint(dir);
        auto supportDist2 = math::distance2(firstCenter, shapeSupport);
        if (supportDist2 > furthestDist2)
        {
            firstSupport = shapeSupport;
            furthestDist2 = supportDist2;
        }
    }
    firstSupport = (firstTransf * math::vec4(firstSupport, 1.f)).xyz();

    math::vec3 secondSupport = secondCenter;
    furthestDist2 = 0.f;
    dir = secondInvRot * (-direction);

    for (auto& shape : secondCollider.shapes)
    {
        auto shapeSupport = shape->getSupportPoint(dir);
        auto supportDist2 = math::distance2(secondCenter, shapeSupport);
        if (supportDist2 > furthestDist2)
        {
            secondSupport = shapeSupport;
            furthestDist2 = supportDist2;
        }
    }
    secondSupport = (secondTransf * math::vec4(secondSupport, 1.f)).xyz();

    return firstSupport - secondSupport;
}


static float calculateCollision(
    collider& firstCollider, const math::vec3& firstCenter, const math::mat4& firstTransf, const math::mat3& firstInvRot,
    collider& secondCollider, const math::vec3& secondCenter, const math::mat4& secondTransf, const math::mat3& secondInvRot,
    const math::vec3& direction)
{
    math::vec3 firstSupport = firstCenter;
    float furthestDist2 = 0.f;
    math::vec3 dir = firstInvRot * direction;

    for (auto& shape : firstCollider.shapes)
    {
        auto shapeSupport = shape->getSupportPoint(dir);
        auto supportDist2 = math::distance2(firstCenter, shapeSupport);
        if (supportDist2 > furthestDist2)
        {
            firstSupport = shapeSupport;
            furthestDist2 = supportDist2;
        }
    }
    firstSupport = (firstTransf * math::vec4(firstSupport, 1.f)).xyz();

    math::vec3 secondSupport = secondCenter;
    furthestDist2 = 0.f;
    dir = secondInvRot * (-direction);

    for (auto& shape : secondCollider.shapes)
    {
        auto shapeSupport = shape->getSupportPoint(dir);
        auto supportDist2 = math::distance2(secondCenter, shapeSupport);
        if (supportDist2 > furthestDist2)
        {
            secondSupport = shapeSupport;
            furthestDist2 = supportDist2;
        }
    }
    secondSupport = (secondTransf * math::vec4(secondSupport, 1.f)).xyz();

    return math::dot(firstSupport - secondSupport, direction);
}


static bool getCollisionNormal(
    collider& firstCollider, const math::vec3& firstCenter, const math::mat4& firstTransf, const math::mat3& firstInvRot,
    collider& secondCollider, const math::vec3& secondCenter, const math::mat4& secondTransf, const math::mat3& secondInvRot,
    const simplex& smplx, collision_normal& minNormal)
{
    auto getSupportPoint = [&](const math::vec3& dir)
    {
        return calculateSupportPoint(
            firstCollider, firstCenter, firstTransf, firstInvRot,
            secondCollider, secondCenter, secondTransf, secondInvRot,
            dir);
    };

    std::vector<math::vec3> polytope(smplx.begin(), smplx.end());
    std::vector<size_type>  faces = {
        0, 1, 2,
        0, 3, 1,
        0, 2, 3,
        1, 3, 2
    };
    std::vector<collision_normal> normals;
    size_type minFace;

    getFaceNormals(polytope, faces, normals, minFace);

    minNormal = collision_normal{ math::vec3::zero, std::numeric_limits<float>::max() };

    size_type iterations = 0;
    while (math::close_enough(minNormal.depth, std::numeric_limits<float>::max()))
    {
        if (iterations > 32ull)
            return false;
        iterations++;

        minNormal = normals[minFace];

        math::vec3 support = getSupportPoint(minNormal.axis);

        if (math::abs(math::dot(minNormal.axis, support) - minNormal.depth) > epsilon)
        {
            minNormal.depth = std::numeric_limits<float>::max();

            std::vector<math::vec<2, size_type>> uniqueEdges;

            for (size_t i = 0; i < normals.size(); i++)
            {
                if (math::dot(normals[i].axis, support) > 0.f)
                {
                    size_t f = i * 3;

                    addIfUniqueEdge(uniqueEdges, faces, f, f + 1);
                    addIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
                    addIfUniqueEdge(uniqueEdges, faces, f + 2, f);

                    faces[f + 2] = faces.back();
                    faces.pop_back();

                    faces[f + 1] = faces.back();
                    faces.pop_back();

                    faces[f] = faces.back();
                    faces.pop_back();

                    normals[i] = normals.back();
                    normals.pop_back();

                    i--;
                }
            }

            if (uniqueEdges.empty())
                return false;

            std::vector<size_t> newFaces;
            for (auto& edgeIndices : uniqueEdges)
            {
                newFaces.push_back(edgeIndices[0]);
                newFaces.push_back(edgeIndices[1]);
                newFaces.push_back(polytope.size());
            }

            polytope.push_back(support);


            std::vector<collision_normal> newNormals;

            size_type newMinTri;
            getFaceNormals(polytope, newFaces, newNormals, newMinTri);

            float newMinDistance = std::numeric_limits<float>::max();
            for (size_t i = 0; i < normals.size(); i++)
                if (normals[i].depth < newMinDistance)
                {
                    newMinDistance = normals[i].depth;
                    minFace = i;
                }

            if (newNormals[newMinTri].depth < newMinDistance)
            {
                minFace = newMinTri + normals.size();
            }

            faces.insert(faces.end(), newFaces.begin(), newFaces.end());
            normals.insert(normals.end(), newNormals.begin(), newNormals.end());
        }
    }

    return true;
}

bool PhysicsSystem::checkCollision(const collision_pair& pair, collision& data)
{
    collider& firstCollider = pair.firstCollider;
    auto firstLocalCenter = firstCollider.bounds.center();
    auto& firstTransf = pair.firstTransf;
    auto& firstInvRot = pair.firstInvRot;
    auto firstWorldPos = firstTransf[3].xyz();

    collider& secondCollider = pair.secondCollider;
    auto secondLocalCenter = secondCollider.bounds.center();
    auto& secondTransf = pair.secondTransf;
    auto& secondInvRot = pair.secondInvRot;
    auto secondWorldPos = secondTransf[3].xyz();

    auto getSupportPoint = [&](const math::vec3& dir)
    {
        return calculateSupportPoint(
            firstCollider, firstLocalCenter, firstTransf, firstInvRot,
            secondCollider, secondLocalCenter, secondTransf, secondInvRot,
            dir);
    };

    auto toSecond = safeNormalize((secondWorldPos + secondLocalCenter) - (firstWorldPos + firstLocalCenter));

    auto dist = calculateCollision(
        firstCollider, firstLocalCenter, firstTransf, firstInvRot,
        secondCollider, secondLocalCenter, secondTransf, secondInvRot,
        toSecond);

    if (dist >= 0)
    {
        data.normal = collision_normal{ toSecond, dist };
        data.first = pair.first;
        data.second = pair.second;
        data.firstCollider = { &firstCollider };
        data.secondCollider = { &secondCollider };
        return true;
    }

    return false;

    simplex smplx;
    //smplx.push_front();

    math::vec3 direction;// = safeNormalize(-smplx.at(0));

    size_type iterations = 0;
    do
    {
        if (iterations > 32ull)
            return false;
        iterations++;

        auto supportPoint = getSupportPoint(direction);
        if (math::dot(supportPoint, direction) <= 0)
            return false;

        smplx.push_front(supportPoint);
    } while (!smplx.next(direction));

    if (getCollisionNormal(
        firstCollider, firstLocalCenter, firstTransf, firstInvRot,
        secondCollider, secondLocalCenter, secondTransf, secondInvRot,
        smplx, data.normal))
    {
        data.first = pair.first;
        data.second = pair.second;
        data.firstCollider = { &firstCollider };
        data.secondCollider = { &secondCollider };

        return true;
    }

    return false;
}

void PhysicsSystem::integrateRigidbodies(float deltaTime)
{
    for (auto& ent : m_rigidbodyQuery)
    {
        rigidbody& rb = ent.get_component<rigidbody>().get();
        auto posHandle = ent.get_component<position>();
        auto rotHandle = ent.get_component<rotation>();

        rotation rot;
        if (rotHandle)
        {
            ////-------------------- update angular velocity ------------------//
            math::vec3 angularAcc = rb.torqueAccumulator * rb.globalInverseInertiaTensor;
            rb.angularVelocity += (angularAcc)*deltaTime;

            ////-------------------- update rotation ------------------//
            float angle = math::clamp(math::length(rb.angularVelocity), 0.0f, 32.0f);
            float dtAngle = angle * deltaTime;

            if (!math::close_enough(dtAngle, 0.f))
            {
                math::vec3 axis = safeNormalize(rb.angularVelocity);

                math::quat glmQuat = math::angleAxis(dtAngle, axis);
                rot = math::normalize(glmQuat * rotHandle.get());
            }
        }

        position pos;
        if (posHandle)
        {
            ////-------------------- update velocity ------------------//
            auto mult = 1.f - rb.linearDrag * deltaTime;

            if (mult < 0.0f)
                mult = 0.0f;

            math::vec3 acc = rb.forceAccumulator * rb.inverseMass;
            rb.velocity += acc * deltaTime;
            rb.velocity *= mult;

            ////-------------------- update position ------------------//
            pos = posHandle.get() += rb.velocity * deltaTime;
        }

        rb.resetAccumulators();

        //for now assume that there is no offset from bodyP
        rb.globalCentreOfMass = pos;

        rb.UpdateInertiaTensor(rot);
    }
}
