#pragma once
#include "../engine_include.hpp"

using namespace lgn;

struct [[reflectable]] bounding_box
{
    math::vec3 min;
    math::vec3 max;

    math::vec3 size()
    {
        return max - min;
    }

    math::vec3 center()
    {
        return min + (size() * 0.5f);
    }

    void set_origin(const math::vec3& origin)
    {
        auto halfSize = size() * .5f;
        min = origin - halfSize;
        max = origin + halfSize;
    }

    void expand(const bounding_box& other)
    {
        min = math::min(min, other.min);
        max = math::max(max, other.max);
    }
};
