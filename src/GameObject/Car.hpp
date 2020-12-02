#pragma once

#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <Box2D/Box2D.h>

#include "DynamicObject.hpp"
#include "../constants.hpp"
#include "CarData.hpp"
#include "../settings/Settings.hpp"
class Tire;
#include "Tire.hpp"

class Car : public DynamicObject {
public:
    Car(std::vector<sf::Int32> ids, b2World *world, CarData carData, Settings* settings);
    ~Car();

    void UpdateFriction(float friction);

    void SetState(b2Transform transform, b2Vec2 velocity, float angularVelocity, float steeringAngle);
    void SetTransform(b2Vec2 pos, float angle);

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
    bool isLocalPlayer_;
private:
    CarData carData_;
    b2RevoluteJoint *f1Joint_, *f2Joint_;
    std::vector<Tire*> tires_;
    void PrivateUpdate(float dt);

    //Player-input-controlled parameters
    bool isAccelerating_, isBraking_, isTurningLeft_, isTurningRight_;

    float steeringAngle_;

    // Sounds
    sf::SoundBuffer soundBuffer_;
    sf::Sound enginesound_;
};