#include <SFML/Graphics/Sprite.hpp>

#include <iostream>

#include "../constants.hpp"
#include "Car.hpp"

Car::Car(std::string spritePath, b2World *world, int width, int height): DynamicObject(spritePath, world) {
    enginePower_ = 100;
    maxSpeed_ = 40;
    reverseSpeed_ = 20;
    brakingPower_ = 50;
    tireLockAngle_ = 35 * DEG_TO_RAD;
    tireTurnSpeed_ = 160;

    b2PolygonShape pShape;
    pShape.SetAsBox(width/2.0, height/2.0);
    b2FixtureDef fDef;
    fDef.shape = &pShape;    
    fDef.density = 1;
    body_->CreateFixture(&fDef);

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
    Tire *tire = new Tire("../res/tire.png", world, this);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( -0.8, 1.1 );
    f1Joint_ =(b2RevoluteJoint*)world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    tire = new Tire("../res/tire.png", world, this);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( 0.8, 1.1 );
    f2Joint_ = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    tire = new Tire("../res/tire.png", world, this);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( -0.8, -1.7 );
    world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    tire = new Tire("../res/tire.png", world, this);
    jointDef.bodyB = tire->body_;
    jointDef.localAnchorA.Set( 0.8, -1.7 );
    world->CreateJoint( &jointDef );
    tires_.push_back(tire);

    // Set sprite scale
    sprite_.setScale(PIXELS_PER_METER * width / sprite_.getLocalBounds().width, PIXELS_PER_METER * height / sprite_.getLocalBounds().height);
}

Car::~Car() {
    /*
    for(Tire *t: tires_) {
        delete t;
    }*/
}

void Car::PrivateUpdate(float dt) {

    //All-wheel drive
    for(Tire *t: tires_) {
        t->UpdateFriction();
        t->UpdateDrive(isAccelerating_, isBraking_);
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
    float desiredAngle = 0;
    if(isTurningLeft_) {
        desiredAngle = tireLockAngle_;
    } else if(isTurningRight_) {
        desiredAngle = -tireLockAngle_;
    }
    float currentAngle = f1Joint_->GetJointAngle();
    float turnSpeed = tireTurnSpeed_ * dt * DEG_TO_RAD;
    float deltaAngle = b2Clamp( desiredAngle - currentAngle, -turnSpeed, turnSpeed );
    float newAngle = currentAngle + deltaAngle;
    f1Joint_->SetLimits( newAngle, newAngle );
    f2Joint_->SetLimits( newAngle, newAngle );
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
    return enginePower_;
}

void Car::SetEnginePower(float newPower) {
    enginePower_ = newPower;
}

float Car::GetMaxSpeed() const {
    return maxSpeed_;
}

void Car::SetMaxSpeed(float newSpeed) {
    maxSpeed_ = newSpeed;
}

float Car::GetReverseSpeed() const {
    return reverseSpeed_;
}

void Car::SetReverseSpeed(float newSpeed) {
    reverseSpeed_ = newSpeed;
}

float Car::GetBrakingPower() const {
    return brakingPower_;
}

void Car::SetBrakingPower(float newPower) {
    brakingPower_ = newPower;
}

std::vector<Tire*> Car::GetTires() {
    return tires_;
}