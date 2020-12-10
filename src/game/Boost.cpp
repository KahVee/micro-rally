#include "Boost.hpp"
#include <iostream>


Boost::Boost(sf::Int32 id, std::string spritePath, b2World *world, sf::RenderWindow* window, float magnitude): DynamicObject(id, spritePath, world, window), magnitude_(magnitude)
{
    b2PolygonShape pShape;
    float width = 1.0f;
    float height = 1.0f;
    pShape.SetAsBox(width/2,height/2);
    pShape.SetAsBox(0.5,0.5);
    shape_ = pShape;

    b2FixtureDef fDef;
    fDef.isSensor = true;
    fDef.shape = &shape_;
    b2Fixture* fixture = body_->CreateFixture(&fDef);
    sprite_.setScale(width / sprite_.getLocalBounds().width, height / sprite_.getLocalBounds().height);

    fixture->SetUserData(this);
}

Boost::~Boost() {
    world_->DestroyBody(body_);
}

float Boost::GetMagnitude() {
    return magnitude_;
}

void Boost::SetMagnitude(float magnitude) {
    magnitude_ = magnitude;
}

void Boost::PrivateUpdate(float dt)
{

}