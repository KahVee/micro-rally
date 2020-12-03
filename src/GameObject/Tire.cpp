#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

#include "Tire.hpp"

Tire::Tire(sf::Int32 id, std::string spritePath, b2World *world, Car *car, Settings* settings): DynamicObject(id, spritePath, world, settings) {
    car_ = car;

    float width = 0.3f;
    float height = 0.5f;
    b2PolygonShape pShape;
    pShape.SetAsBox(width * 0.5f, height * 0.5f);
    shape_ = pShape;
    b2FixtureDef fDef;
    fDef.shape = &shape_;
    fDef.density = 1;
    fDef.friction = 1;
    fDef_ = fDef;
    body_->CreateFixture(&fDef_);
    sprite_.setScale(width / sprite_.getLocalBounds().width, height / sprite_.getLocalBounds().height);
}

Tire::~Tire() {
    world_->DestroyBody(body_);
}

void Tire::UpdateFriction(float friction) {
    frictionMultiplier_ = friction;
}

void Tire::UpdateDrive(bool isAccelerating, bool isBraking) {
    float targetSpeed = 0;
    float force = 0;
    if(isAccelerating) {

        //if(lateralvelocity > X ) { asd } else applyimpulse
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
        body_->ApplyForce(frictionMultiplier_ * force * body_->GetWorldVector(b2Vec2(0,1)), body_->GetWorldCenter(), true);
    }

    //TODO: Test ApplyForce() too
    //Stops tire from sliding sideways, TODO: add a variable multiplier
    body_->ApplyLinearImpulse(frictionMultiplier_ * angularVelocityDampeningMultiplier * car_->body_->GetMass() * -LateralVelocity() * body_->GetWorldVector(b2Vec2(1,0)), body_->GetWorldCenter(), true);
    //"Rolling resistance" to stop car from sliding forever
    body_->ApplyForce(frictionMultiplier_ * car_->body_->GetMass() * dragForceMultiplier * -ForwardVelocity() * body_->GetWorldVector(b2Vec2(0,1)), body_->GetWorldCenter(), true);
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