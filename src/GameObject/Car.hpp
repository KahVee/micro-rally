#pragma once

#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <Box2D/Box2D.h>

#include "DynamicObject.hpp"
class Tire;
#include "Tire.hpp"

class Car : public DynamicObject {
public:
    Car(std::string spritePath, b2World *world, int width, int height);
    virtual ~Car();

    void Accelerate(bool in);
    void Brake(bool in);
    void TurnLeft(bool in);
    void TurnRight(bool in);

    float GetEnginePower() const;
    void SetEnginePower(float newPower);
    float GetMaxSpeed() const;
    void SetMaxSpeed(float newSpeed);
    float GetReverseSpeed() const;
    void SetReverseSpeed(float newSpeed);
    float GetBrakingPower() const;
    void SetBrakingPower(float newPower);

    float GetSteeringAngle() const;
    void SetSteeringAngle(float steeringAngle);
    std::vector<Tire*> GetTires();

private:
    b2RevoluteJoint *f1Joint_, *f2Joint_;
    float enginePower_;
    float maxSpeed_;
    float reverseSpeed_;
    float brakingPower_;
    //Front tire lock angle
    float tireLockAngle_;
    //Front tire turning speed in degrees per time-step
    float tireTurnSpeed_;
    std::vector<Tire*> tires_;
    void PrivateUpdate(float dt);

    //Player-input-controlled parameters
    bool isAccelerating_, isBraking_, isTurningLeft_, isTurningRight_ = false;

    float steeringAngle_;
};