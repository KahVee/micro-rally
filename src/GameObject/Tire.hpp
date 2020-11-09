#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>

#include "DynamicObject.hpp"
class Car;
#include "Car.hpp"

class Tire : public DynamicObject {
    public:
        Tire(std::string spritePath, b2World *world, Car *car);
        virtual ~Tire();

        void UpdateFriction();
        void UpdateDrive(bool isAccelerating, bool isBraking);
        void UpdateTurningTorque(bool isTurningLeft, bool isTurningRight);

    private:
        Car *car_;
        b2PolygonShape shape_;
        b2FixtureDef fDef_;
        const float angularVelocityDampeningMultiplier = 10;
        const float dragForceMultiplier = 2;
        void PrivateUpdate(float dt);

        float ForwardVelocity();
        float LateralVelocity();
};