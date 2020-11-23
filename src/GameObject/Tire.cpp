#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

#include "Tire.hpp"

Tire::Tire(sf::Int32 id, std::string spritePath, b2World *world, Car *car): DynamicObject(id, spritePath, world) {
    car_ = car;

    b2PolygonShape pShape;
    pShape.SetAsBox(0.15, 0.25);
    shape_ = pShape;
    b2FixtureDef fDef;
    fDef.shape = &shape_;    
    fDef.density = 1;
    fDef.friction = 1;
    fDef_ = fDef;
    body_->CreateFixture(&fDef_);
}

Tire::~Tire() {
    world_->DestroyBody(body_);
}

void Tire::UpdateFriction() {
    //TODO: Test ApplyForce() too
    //Stops tire from sliding sideways, TODO: add a variable multiplier
    body_->ApplyLinearImpulse(body_->GetMass() * -LateralVelocity() * body_->GetWorldVector(b2Vec2(1,0)), body_->GetWorldCenter(), true);
    //"Rolling resistance", TODO: add surface traction to this
    body_->ApplyForce(-dragForceMultiplier * ForwardVelocity() * body_->GetWorldVector(b2Vec2(0,1)), body_->GetWorldCenter(), true);
}

void Tire::UpdateDrive(bool isAccelerating, bool isBraking) {
    float targetSpeed = 0;
    float force = 0;
    if(isAccelerating) {
        targetSpeed = car_->GetMaxSpeed();
        if(targetSpeed > ForwardVelocity()) {
            force = car_->GetEnginePower();
        } 
    }
    if(isBraking) {
        targetSpeed = -car_->GetReverseSpeed();
        if(targetSpeed < ForwardVelocity()) {
            force = -car_->GetBrakingPower();
        }
    }

    if(force != 0) {
        body_->ApplyForce(/*TRACTION*/ force * body_->GetWorldVector(b2Vec2(0,1)), body_->GetWorldCenter(), true);
    }
}

//NOT USED ATM
void Tire::UpdateTurningTorque(bool isTurningLeft, bool isTurningRight) {
    float torque = 0;
    if(isTurningLeft) {
        torque = 15;
    }
    if(isTurningRight) {
        torque = -15;
    }
    if(torque != 0) {
        body_->ApplyTorque(torque, true);
    }
}

void Tire::PrivateUpdate(float dt) {
    
}

float Tire::ForwardVelocity() {
    return b2Dot(body_->GetWorldVector(b2Vec2(0,1)), body_->GetLinearVelocity());
}

float Tire::LateralVelocity() {
    return b2Dot(body_->GetWorldVector(b2Vec2(1,0)), body_->GetLinearVelocity());
}