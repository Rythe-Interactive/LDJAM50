#pragma once
#include "../engine_include.hpp"

namespace legion::core
{
    struct [[no_reflect]] example_policy : public particle_policy<example_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(example_policy);
        ~example_policy() = default;

        virtual void onInit(particle_emitter& emitter, size_type start, size_type end) override;
    };

    struct [[no_reflect]] orbital_policy : public particle_policy<orbital_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(orbital_policy);
        ~orbital_policy() = default;

       double C_MASS = 10.f;
       double P_MASS = 1.f;
       double G_FORCE = 100.0f;

        virtual void setup(particle_emitter& emitter) override;
        virtual void onInit(particle_emitter& emitter, size_type start, size_type end) override;
        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    };

    struct [[no_reflect]] fountain_policy : public particle_policy<fountain_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(fountain_policy);
        ~fountain_policy() = default;

        float initForce;

        virtual void onInit(particle_emitter& emitter, size_type start, size_type end) override;
        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    };

    struct [[no_reflect]] scale_lifetime_policy : public particle_policy<scale_lifetime_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(scale_lifetime_policy);
        ~scale_lifetime_policy() = default;

        virtual void setup(particle_emitter & emitter) override;
        virtual void onInit(particle_emitter & emitter, size_type start, size_type end) override;
        virtual void onUpdate(particle_emitter & emitter, float deltaTime, size_type count) override;
    };

    struct [[no_reflect]] color_lifetime_policy : public particle_policy<color_lifetime_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(color_lifetime_policy);
        ~color_lifetime_policy() = default;

        virtual void setup(particle_emitter& emitter) override;
        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    };

    struct bounds
    {
        position center;
        position min;
        position max;
        float border;
    };

    struct [[no_reflect]] seperation_policy : public particle_policy<seperation_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(seperation_policy);
        ~seperation_policy() = default;

        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    };

    struct [[no_reflect]] alignment_policy : public particle_policy<alignment_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(alignment_policy);
        ~alignment_policy() = default;

        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    };

    struct [[no_reflect]] cohesion_policy : public particle_policy<cohesion_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(cohesion_policy);
        ~cohesion_policy() = default;

        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    };

    struct [[no_reflect]] locomotion_policy : public particle_policy<locomotion_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(locomotion_policy);
        ~locomotion_policy() = default;

        virtual void setup(particle_emitter& emitter) override;
        virtual void onInit(particle_emitter& emitter, size_type start, size_type end) override;
        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    };


    struct [[no_reflect]] explosion_policy : public particle_policy<explosion_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(explosion_policy);
        ~explosion_policy() = default;

        virtual void setup(particle_emitter& emitter) override;
        virtual void onInit(particle_emitter& emitter, size_type start, size_type end) override;
        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    };
}
