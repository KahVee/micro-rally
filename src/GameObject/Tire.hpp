#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include "DynamicObject.hpp"

class Tire : public DynamicObject {
    public:
        Tire(sf::Sprite sprite, b2World *world);
        virtual ~Tire();

        void UpdateFriction();
        void DebugApplyForce();

    private:
        const float AngularVelocityMultiplier = 0.1f;
};