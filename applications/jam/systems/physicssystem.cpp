#include "physicssystem.hpp"
#include "../data/simplex.hpp"

std::unique_ptr<BroadPhase> PhysicsSystem::m_boardPhase(nullptr);

void PhysicsSystem::setup()
{
    if (!m_boardPhase)
        m_boardPhase = std::make_unique<BruteForce>();

    createProcess<&PhysicsSystem::fixedUpdate>("Update", 0.02f);
}

void PhysicsSystem::fixedUpdate(time::time_span<fast_time> deltaTime)
{
    checkCollisions(deltaTime);

    integrateRigidbodies(deltaTime);
}

void PhysicsSystem::checkCollisions(float deltaTime)
{
    std::vector<collision_pair> pairs;
    m_boardPhase->sortPairs(pairs);

    for (auto& pair : pairs)
    {
        if (checkCollision(pair))
            log::debug("Collision between {} {}", pair.first->name, pair.second->name);
    }
}

static math::vec3 calculateSupportPoint(
    collider& firstCollider, const math::vec3& firstCenter, const math::vec3& firstPos, const math::quat& firstRot, const math::vec3& firstScale,
    collider& secondCollider, const math::vec3& secondCenter, const math::vec3& secondPos, const math::quat& secondRot, const math::vec3& secondScale,
    const math::vec3& direction)
{
    math::vec3 firstSupport = firstCenter;
    float furthestDist2 = 0.f;
    math::vec3 dir = math::inverse(firstRot) * direction;

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
    firstSupport = firstPos + ((firstRot * firstSupport) * firstScale);

    math::vec3 secondSupport = secondCenter;
    furthestDist2 = 0.f;
    dir = math::inverse(secondRot) * -direction;

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
    secondSupport = secondPos + ((secondRot * firstSupport) * secondScale);

    return firstSupport - secondSupport;
}

bool PhysicsSystem::checkCollision(const collision_pair& pair)
{
    collider& firstCollider = pair.firstCollider;
    auto firstLocalCenter = firstCollider.bounds.center();
    auto& firstWorldPos = pair.firstPosition;
    auto& firstWorldRotation = pair.firstRotation;
    auto& firstWorldScale = pair.firstScale;


    collider& secondCollider = pair.secondCollider;
    auto secondLocalCenter = secondCollider.bounds.center();
    auto& secondWorldPos = pair.secondPosition;
    auto& secondWorldRotation = pair.secondRotation;
    auto& secondWorldScale = pair.secondScale;

    auto getSupportPoint = [&](const math::vec3& dir)
    {
        return calculateSupportPoint(
            firstCollider, firstLocalCenter, firstWorldPos, firstWorldRotation, firstWorldScale,
            secondCollider, secondLocalCenter, secondWorldPos, secondWorldRotation, secondWorldScale,
            dir);
    };

    simplex smplx;
    smplx.push_front(getSupportPoint(math::normalize((firstWorldPos + firstLocalCenter) - (secondWorldPos + secondLocalCenter))));

    math::vec3 direction = math::normalize(-smplx.at(0));

    do
    {
        auto supportPoint = getSupportPoint(direction);
        if (math::dot(supportPoint, direction) < 0)
            return false;

        smplx.push_front(supportPoint);
    } while (!smplx.next(direction));

    return true;
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

            if (!math::epsilonEqual(dtAngle, 0.0f, math::epsilon<float>()))
            {
                math::vec3 axis = math::normalize(rb.angularVelocity);

                math::quat glmQuat = math::angleAxis(dtAngle, axis);
                rot = math::normalize(glmQuat * rotHandle.get());
            }
        }

        position pos;
        if (posHandle)
        {
            ////-------------------- update velocity ------------------//
            auto mult = 1.f - rb.linearDrag * deltaTime;
            if (mult < 0.0f) mult = 0.0f;
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
