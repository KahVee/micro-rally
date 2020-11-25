#include "tirestack.hpp"
#include <iostream>

Tirestack::Tirestack(sf::Int32 id, std::string spritePath, b2World *world) : DynamicObject(id, spritePath, world)
{
    body_->SetType(b2_staticBody);
    b2CircleShape pShape;
    pShape.m_p.Set(0, 0);
    pShape.m_radius = 0.5;
    shape_ = pShape;
    b2FixtureDef fDef;
    fDef.shape = &shape_;
    fDef.density = 1;
    fDef.friction = 1;
    fDef_ = fDef;
    body_->CreateFixture(&fDef_);
}

Tirestack::~Tirestack()
{
    world_->DestroyBody(body_);
}

void Tirestack::PrivateUpdate(float dt)
{
    std::cout << GetTransform().p.x << " " << GetTransform().p.y << " Olen taalla" << std::endl;
    body_->ApplyForceToCenter(10 * frictionMultiplier_ * -GetVelocity(), true);
    body_->ApplyTorque(10 * frictionMultiplier_ * -GetAngularVelocity(), true);
}