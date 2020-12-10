#include "Box.hpp"
#include <iostream>

Box::Box(sf::Int32 id, std::string spritePath, b2World *world, sf::RenderWindow* window, float size): DynamicObject(id, spritePath, world, window)
{
    b2PolygonShape pShape;
    pShape.SetAsBox(size/2,size/2);
    shape_ = pShape;
    b2FixtureDef fDef;
    fDef.shape = &shape_;
    fDef.density = 1;
    fDef.friction = 1;
    fDef_ = fDef;
    b2Fixture* fixture = body_->CreateFixture(&fDef_);
    sprite_.setScale(size / sprite_.getLocalBounds().width, size / sprite_.getLocalBounds().height);
    fixture->SetUserData(this);
}
Box::~Box() {
    world_->DestroyBody(body_);
}

void Box::PrivateUpdate(float dt)
{
    //std::cout << GetTransform().p.x << " " << GetTransform().p.y << " Olen taalla" << std::endl;
    body_->ApplyForceToCenter(8 * frictionMultiplier_ * -GetVelocity(), true);
    body_->ApplyTorque(4 * frictionMultiplier_ * -GetAngularVelocity(), true);
}

