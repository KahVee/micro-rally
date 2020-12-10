#include "Box.hpp"
#include <iostream>

Box::Box(sf::Int32 id, std::string spritePath, b2World *world, sf::RenderWindow* window): DynamicObject(id, spritePath, world, window)
{
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
    b2Fixture* fixture = body_->CreateFixture(&fDef_);
    sprite_.setScale(width / sprite_.getLocalBounds().width, height / sprite_.getLocalBounds().height);
    fixture->SetUserData(this);
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

