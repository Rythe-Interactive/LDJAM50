#pragma once
#include "../engine_include.hpp"
#include "../data/bounding_box.hpp"

using namespace lgn;

class CollisionShape
{
protected:
    math::vec3 m_offset;
    math::vec3 m_scale = math::vec3(1.f);

    virtual void recalculateBounds() LEGION_PURE;

public:
    bounding_box bounds = bounding_box();

    NO_DTOR_RULE5_NOEXCEPT(CollisionShape);

    CollisionShape(const math::vec3& _offset, const math::vec3& _scale) noexcept
        : m_offset(_offset), m_scale(_scale), bounds() {}

    virtual ~CollisionShape() noexcept = default;

    virtual math::vec3 getSupportPoint(const math::vec3& direction) LEGION_PURE;
    virtual id_type typeId() const noexcept LEGION_PURE;
    virtual cstring typeName() const noexcept LEGION_PURE;

    virtual math::vec3 getOffset() const noexcept LEGION_IMPURE_RETURN(m_offset);
    virtual void setOffset(const math::vec3& val) { m_offset = val; recalculateBounds(); }
    virtual math::vec3 getScale() const noexcept LEGION_IMPURE_RETURN(m_scale);
    virtual void setScale(const math::vec3& val) { m_scale = val; recalculateBounds(); }
};

class SphereCollider : public CollisionShape
{
protected:
    float m_radius;

    virtual void recalculateBounds()
    {
        auto radii = m_scale * m_radius;
        bounds.min = m_offset - radii;
        bounds.max = m_offset + radii;
    }

public:
    SphereCollider() noexcept
        : CollisionShape(), m_radius(0.5f)
    {
        recalculateBounds();
    }

    SphereCollider(float _radius) noexcept
        : CollisionShape(), m_radius(_radius)
    {
        recalculateBounds();
    }

    SphereCollider(const math::vec3& _offset, const math::vec3& _scale, float _radius) noexcept
        : CollisionShape(_offset, _scale), m_radius(_radius)
    {
        recalculateBounds();
    }

    MOVE_FUNCS_NOEXCEPT(SphereCollider);
    COPY_FUNCS_NOEXCEPT(SphereCollider);

    virtual ~SphereCollider() noexcept = default;

    virtual math::vec3 getSupportPoint(const math::vec3& direction)
    {
        return m_offset + (direction * m_radius * m_scale);
    }

    float getRadius() const noexcept { return m_radius; }
    void setRadius(float _radius) noexcept { m_radius = _radius; recalculateBounds(); }

    virtual id_type typeId() const noexcept { return typeHash<SphereCollider>(); }
    virtual cstring typeName() const noexcept { return "Sphere"; }
};

class BoxCollider : public CollisionShape
{
protected:
    math::vec3 m_size;

    virtual void recalculateBounds()
    {
        auto radii = m_scale * m_size * 0.5f;
        bounds.min = m_offset - radii;
        bounds.max = m_offset + radii;
    }

public:
    BoxCollider() noexcept
        : CollisionShape(), m_size(math::vec3::one)
    {
        recalculateBounds();
    }

    BoxCollider(const math::vec3& _offset, const math::vec3& _scale, math::vec3 _size) noexcept
        : CollisionShape(_offset, _scale), m_size(_size)
    {
        recalculateBounds();
    }

    MOVE_FUNCS_NOEXCEPT(BoxCollider);
    COPY_FUNCS_NOEXCEPT(BoxCollider);

    virtual ~BoxCollider() noexcept = default;

    virtual math::vec3 getSupportPoint(const math::vec3& direction)
    {
        auto size = bounds.size();
        auto maxSize = math::max(size.x, size.y);
        return math::clamp(direction * maxSize * maxSize, bounds.min, bounds.max);
    }

    math::vec3 getSize() const noexcept { return m_size; }
    void setSize(const math::vec3& val) { m_size = val; recalculateBounds(); }

    virtual id_type typeId() const noexcept { return typeHash<BoxCollider>(); }
    virtual cstring typeName() const noexcept { return "Box"; }
};

