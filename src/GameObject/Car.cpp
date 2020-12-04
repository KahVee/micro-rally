#include <SFML/Graphics/Sprite.hpp>

#include <iostream>

#include "../constants.hpp"
#include <cmath>
#include "Car.hpp"

Car::Car(std::vector<sf::Int32> ids, b2World *world, CarData carData, sf::RenderWindow* window)
    :DynamicObject(ids[0], carData.spritePath, world, window), carData_(carData), window_(window) {

    isAccelerating_, isBraking_, isTurningLeft_, isTurningRight_ = false;

    b2PolygonShape pShape;
    pShape.SetAsBox(carData_.bodyWidth/2.0, carData_.bodyHeight/2.0);
    b2FixtureDef fDef;
    fDef.shape = &pShape;    
    fDef.density = carData_.bodyDensity;
    b2Fixture *fixture = body_->CreateFixture(&fDef);
    fixture->SetUserData(this);

    //Constructing the tires
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = body_;
    jointDef.enableLimit = true;
    jointDef.collideConnected = false;
    jointDef.referenceAngle = 0;
    //unrotateable tires
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorB.SetZero();

    //front tires are created first
    Tire *tire = new Tire(ids[1], "../res/tire.png", world, this, window);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( carData_.tirePositions[0].first, carData_.tirePositions[0].second );
    f1Joint_ =(b2RevoluteJoint*)world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    tire = new Tire(ids[2], "../res/tire.png", world, this, window);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( carData_.tirePositions[1].first, carData_.tirePositions[1].second );
    f2Joint_ = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    tire = new Tire(ids[3], "../res/tire.png", world, this, window);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( carData_.tirePositions[2].first, carData_.tirePositions[2].second );
    world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    tire = new Tire(ids[4], "../res/tire.png", world, this, window);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( carData_.tirePositions[3].first, carData_.tirePositions[3].second );
    world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    // Set sprite scale
    sprite_.setScale(carData_.bodyWidth / sprite_.getLocalBounds().width, carData_.bodyHeight / sprite_.getLocalBounds().height);
    steeringAngle_ = 0.f;

    // Sounds
    if(!soundBuffer_.loadFromFile("../res/audio/enginesound.wav"))
    {
        // TODO WHEN SOUND DOESN'T LOAD
    }
    enginesound_.setBuffer(soundBuffer_);
    enginesound_.setLoop(true);
    enginesound_.setVolume(0.f);
    enginesound_.setMinDistance(20.f);
    enginesound_.setAttenuation(0.5f);
    enginesound_.play();
}

Car::~Car() {
    world_->DestroyBody(body_);
}

void Car::SetTransform(b2Vec2 pos, float angle) {
    body_->SetTransform(pos, angle);
    for(int i = 0; i < 4; i++) {
        b2Vec2 originalPos = b2Vec2(carData_.tirePositions[i].first, carData_.tirePositions[i].second);
        b2Vec2 newPos = body_->GetWorldPoint(b2Vec2(carData_.tirePositions[i].first, carData_.tirePositions[i].second));
        tires_[i]->SetTransform(newPos, angle);
    }
}

void Car::PrivateUpdate(float dt) {
    //All-wheel drive
    for(Tire *t: tires_) {
        t->UpdateDrive(isAccelerating_, isBraking_);
        t->Update(dt);
    }

    //Rear-wheel drive, steering not working yet...
    /*
    tires_[2]->UpdateFriction();
    tires_[2]->UpdateDrive(isAccelerating_, isBraking_);
    tires_[3]->UpdateFriction();
    tires_[3]->UpdateDrive(isAccelerating_, isBraking_);
    */
    //Front tire torque, makes driving more responsive but rotates the whole car when stationary
    //tires_[0]->UpdateTurningTorque(isTurningLeft_, isTurningRight_);
    //tires_[1]->UpdateTurningTorque(isTurningLeft_, isTurningRight_); 

    //Force the front tires to a specific turn angle.
    if(isLocalPlayer_ && (isTurningLeft_ || isTurningRight_) ) {
        float desiredAngle = 0;
        if(isTurningLeft_) {
            desiredAngle = carData_.tireLockAngle * DEG_TO_RAD;
        } else if(isTurningRight_) {
            desiredAngle = -carData_.tireLockAngle * DEG_TO_RAD;
        }
        float currentAngle = steeringAngle_;
        float turnSpeed = carData_.tireTurnSpeed * dt * DEG_TO_RAD;
        float deltaAngle = b2Clamp( desiredAngle - currentAngle, -turnSpeed, turnSpeed );
        steeringAngle_ = currentAngle + deltaAngle;
    } else {
        steeringAngle_ = 0;
    }

    f1Joint_->SetLimits( steeringAngle_, steeringAngle_ );
    f2Joint_->SetLimits( steeringAngle_, steeringAngle_ );

    // Sounds
    float speed = sqrt(GetVelocity().x * GetVelocity().x + GetVelocity().y * GetVelocity().y);
    if(speed > 0.5f || speed < -0.5f)
    {
        enginesound_.setVolume(15.f);
    }else
    {
        enginesound_.setVolume(0.f);
    }
    
    float pitch = speed / GetMaxSpeed();
    enginesound_.setPitch(0.5f + pitch);
    enginesound_.setPosition(GetTransform().p.x, 0.f, window_->getSize().y - GetTransform().p.y);
}

void Car::SetState(b2Transform transform, b2Vec2 velocity, float angularVelocity, float steeringAngle) {
    SetTransform(transform.p, transform.q.GetAngle());
    SetVelocity(velocity);
    SetAngularVelocity(angularVelocity);
    SetSteeringAngle(steeringAngle);    
}

void Car::ApplyBoost(float boostScale) {
    body_->ApplyLinearImpulse(body_->GetWorldVector(b2Vec2(0,boostScale)), body_->GetWorldCenter(), true);
}
void Car::ApplySpin(float spinScale) {
    int dir = rand()%2 ? 1 : -1;
    body_->ApplyLinearImpulse(dir * body_->GetWorldVector(b2Vec2(spinScale, 0)), body_->GetWorldPoint(b2Vec2(0,carData_.bodyHeight/2)), true);
}

void Car::Accelerate(bool in) {
    isAccelerating_ = in;
}

void Car::Brake(bool in) {
    isBraking_ = in;
}

void Car::TurnLeft(bool in) {
    isTurningLeft_ = in;
}

void Car::TurnRight(bool in) {
    isTurningRight_ = in;
}

float Car::GetEnginePower() const {
    return carData_.enginePower;
}

void Car::SetEnginePower(float newPower) {
    carData_.enginePower = newPower;
}

float Car::GetMaxSpeed() const {
    return carData_.maxSpeed;
}

void Car::SetMaxSpeed(float newSpeed) {
    carData_.maxSpeed = newSpeed;
}

float Car::GetReverseSpeed() const {
    return carData_.reverseSpeed;
}

void Car::SetReverseSpeed(float newSpeed) {
    carData_.reverseSpeed = newSpeed;
}

float Car::GetBrakingPower() const {
    return carData_.brakingPower;
}

void Car::SetBrakingPower(float newPower) {
    carData_.brakingPower = newPower;
}

float Car::GetSteeringAngle() const {
    return steeringAngle_;
}

void Car::SetSteeringAngle(float steeringAngle) {
    steeringAngle_ = steeringAngle;
}

sf::Sound& Car::GetEngineSound()
{
    return enginesound_;
}

std::vector<Tire*> Car::GetTires() {
    return tires_;
}

void Car::UpdateFriction(float friction)
{
    for (auto tire: tires_)
    {
        tire->UpdateFriction(friction);
    }
}