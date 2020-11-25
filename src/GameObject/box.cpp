#include "box.hpp"
#include <iostream>

Box::Box(sf::Int32 id, std::string spritePath, b2World *world): DynamicObject(id, spritePath, world) 
{
    b2PolygonShape pShape;
    pShape.SetAsBox(0.5,0.5);
    shape_ = pShape;
    b2FixtureDef fDef;
    fDef.shape = &shape_;
    fDef.density = 1;
    fDef.friction = 1;
    fDef_ = fDef;
    body_->CreateFixture(&fDef_);

}
Box::~Box() {
    world_->DestroyBody(body_);
}

void Box::PrivateUpdate(float dt)
{
    //std::cout << GetTransform().p.x << " " << GetTransform().p.y << " Olen taalla" << std::endl;
    body_->ApplyForceToCenter(8 * frictionMultiplier_ * -GetVelocity(), true);
    body_->ApplyTorque(5 * frictionMultiplier_ * -GetAngularVelocity(), true);
}

