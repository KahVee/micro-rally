#include <SFML/Graphics/Sprite.hpp>

#include <iostream>

#include "../constants.hpp"
#include "Car.hpp"

Car::Car(std::vector<sf::Int32> ids, b2World *world, CarData carData)
    :DynamicObject(ids[0], carData.spritePath, world), carData_(carData) {

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
    Tire *tire = new Tire(ids[1], "../res/tire.png", world, this);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( carData_.tirePositions[0].first, carData_.tirePositions[0].second );
    f1Joint_ =(b2RevoluteJoint*)world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    tire = new Tire(ids[2], "../res/tire.png", world, this);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( carData_.tirePositions[1].first, carData_.tirePositions[1].second );
    f2Joint_ = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    tire = new Tire(ids[3], "../res/tire.png", world, this);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( carData_.tirePositions[2].first, carData_.tirePositions[2].second );
    world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    tire = new Tire(ids[4], "../res/tire.png", world, this);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( carData_.tirePositions[3].first, carData_.tirePositions[3].second );
    world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    // Set sprite scale
    sprite_.setScale(PIXELS_PER_METER * carData_.bodyWidth / sprite_.getLocalBounds().width, PIXELS_PER_METER * carData_.bodyHeight / sprite_.getLocalBounds().height);
    steeringAngle_ = 0;
}

Car::~Car() {
    world_->DestroyBody(body_);
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
    if(isLocalPlayer_) {
        float desiredAngle = 0;
        if(isTurningLeft_) {
            desiredAngle = carData_.tireLockAngle;
        } else if(isTurningRight_) {
            desiredAngle = -carData_.tireLockAngle;
        }
        float currentAngle = f1Joint_->GetJointAngle();
        float turnSpeed = carData_.tireTurnSpeed * dt * DEG_TO_RAD;
        float deltaAngle = b2Clamp( desiredAngle - currentAngle, -turnSpeed, turnSpeed );
        steeringAngle_ = currentAngle + deltaAngle;
    }
    f1Joint_->SetLimits( steeringAngle_, steeringAngle_ );
    f2Joint_->SetLimits( steeringAngle_, steeringAngle_ );
}

void Car::SetState(b2Transform transform, b2Vec2 velocity, float angularVelocity, float steeringAngle) {
    SetTransform(transform.p, transform.q.GetAngle());
    SetVelocity(velocity);
    SetAngularVelocity(angularVelocity);
    SetSteeringAngle(steeringAngle);

    b2Transform t = GetTransform();    
    b2Vec2 pos = body_->GetPosition() + body_->GetWorldVector(b2Vec2(carData_.tirePositions[0].first, carData_.tirePositions[0].second));
    tires_[0]->body_->SetTransform(pos, t.q.GetAngle() + steeringAngle_);
    pos = body_->GetPosition() + body_->GetWorldVector(b2Vec2(carData_.tirePositions[1].first, carData_.tirePositions[1].second));
    tires_[1]->body_->SetTransform(pos, t.q.GetAngle() + steeringAngle_);
    pos = body_->GetPosition() + body_->GetWorldVector(b2Vec2(carData_.tirePositions[2].first, carData_.tirePositions[2].second));
    tires_[2]->body_->SetTransform(pos, t.q.GetAngle());
    pos = body_->GetPosition() + body_->GetWorldVector(b2Vec2(carData_.tirePositions[3].first, carData_.tirePositions[3].second));
    tires_[3]->body_->SetTransform(pos, t.q.GetAngle());
    
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