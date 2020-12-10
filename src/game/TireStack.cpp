#include "TireStack.hpp"
#include <iostream>

TireStack::TireStack(sf::Int32 id, std::string spritePath, b2World *world, sf::RenderWindow* window) : DynamicObject(id, spritePath, world, window)
{
    body_->SetType(b2_staticBody);
    b2CircleShape shape;
    float diameter = 1.0f;
    shape.m_radius = diameter/2;
    shape_ = shape;
    b2FixtureDef fDef;
    fDef.shape = &shape_;
    fDef.density = 1;
    fDef.friction = 1;
    fDef_ = fDef;
    b2Fixture* fixture = body_->CreateFixture(&fDef_);
    
    sprite_.setScale(diameter / sprite_.getLocalBounds().width, diameter / sprite_.getLocalBounds().height);
    fixture->SetUserData(this);
}

TireStack::~TireStack()
{
    world_->DestroyBody(body_);
}

void TireStack::PrivateUpdate(float dt)
{
    //std::cout << GetTransform().p.x << " " << GetTransform().p.y << " Olen taalla" << std::endl;
    body_->ApplyForceToCenter(10 * frictionMultiplier_ * -GetVelocity(), true);
    body_->ApplyTorque(10 * frictionMultiplier_ * -GetAngularVelocity(), true);
}