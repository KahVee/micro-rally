#pragma once

#include <Box2D/Box2D.h>

#include "DynamicObject.hpp"
class Car;
#include "Car.hpp"

class Tire : public DynamicObject {
public:
    Tire(sf::Int32 id, std::string spritePath, b2World *world, Car *car, Settings* settings);
    ~Tire();

    void UpdateFriction(float friction);
    void UpdateDrive(bool isAccelerating, bool isBraking);
    void UpdateTurningTorque(bool isTurningLeft, bool isTurningRight);

private:
    Car *car_;
    b2PolygonShape shape_;
    b2FixtureDef fDef_;
    const float angularVelocityDampeningMultiplier = 0.01;
    const float dragForceMultiplier = 0.1;
    void PrivateUpdate(float dt);

    float ForwardVelocity();
    float LateralVelocity();
};