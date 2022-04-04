#pragma once
#include "../engine_include.hpp"

using namespace lgn;

struct [[reflectable]] bounding_box
{
    math::vec3 min;
    math::vec3 max;

    math::vec3 size() const noexcept
    {
        return max - min;
    }

    math::vec3 center() const noexcept
    {
        return min + (size() * 0.5f);
    }

    void set_origin(const math::vec3& origin)
    {
        auto halfSize = size() * .5f;
        min = origin - halfSize;
        max = origin + halfSize;
    }

    void expand(const bounding_box& other) noexcept
    {
        min = math::min(min, other.min);
        max = math::max(max, other.max);
    }

    void expand(const math::vec3& point) noexcept
    {
        min = math::min(min, point);
        max = math::max(max, point);
    }

    void get_verts(std::array<math::vec3, 8>& arr) const noexcept
    {
        arr[0] = min;
        arr[1] = math::vec3(min.x, min.y, max.z);
        arr[2] = math::vec3(max.x, min.y, max.z);
        arr[3] = math::vec3(max.x, min.y, min.z);
        arr[4] = math::vec3(max.x, max.y, min.z);
        arr[5] = math::vec3(min.x, max.y, min.z);
        arr[6] = math::vec3(min.x, max.y, max.z);
        arr[7] = max;
    }
};
