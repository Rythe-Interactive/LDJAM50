#pragma once
struct [[reflectable]] bullet_comp
{
    float damge = 10.f;
    float lifetime = 5.f;
    float age = 0.f;
    bool fromPlayer = true;
};
