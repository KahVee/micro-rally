#pragma once

#include <Box2D/Box2D.h>

#include "DynamicObject.hpp"
class Car;
#include "Car.hpp"

class Tire : public DynamicObject {
public:
    Tire(sf::Int32 id, std::string spritePath, b2World *world, Car *car);
    ~Tire();

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