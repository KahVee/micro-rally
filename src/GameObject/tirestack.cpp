#include "tirestack.hpp"
#include <iostream>

Tirestack::Tirestack(sf::Int32 id, std::string spritePath, b2World *world, Settings* settings) : DynamicObject(id, spritePath, world, settings)
{
    body_->SetType(b2_staticBody);
    b2PolygonShape pShape;
    float width = 1.0f;
    float height = 1.0f;
    pShape.SetAsBox(width/2,height/2);
    shape_ = pShape;
    b2FixtureDef fDef;
    fDef.shape = &shape_;
    fDef.density = 1;
    fDef.friction = 1;
    fDef_ = fDef;
    body_->CreateFixture(&fDef_);
    sprite_.setScale(width / sprite_.getLocalBounds().width, height / sprite_.getLocalBounds().height);
}

Tirestack::~Tirestack()
{
    world_->DestroyBody(body_);
}

void Tirestack::PrivateUpdate(float dt)
{
    //std::cout << GetTransform().p.x << " " << GetTransform().p.y << " Olen taalla" << std::endl;
    body_->ApplyForceToCenter(10 * frictionMultiplier_ * -GetVelocity(), true);
    body_->ApplyTorque(10 * frictionMultiplier_ * -GetAngularVelocity(), true);
}