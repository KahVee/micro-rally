#include "boost.hpp"
#include <iostream>


Boost::Boost(sf::Int32 id, std::string spritePath, b2World *world): DynamicObject(id, spritePath, world) 
{
    b2PolygonShape pShape;
    pShape.SetAsBox(0.5,0.5);
    shape_ = pShape;

    b2FixtureDef fDef;
    fDef.isSensor = true;
    fDef.shape = &shape_;
    b2Fixture* fixture = body_->CreateFixture(&fDef);

    fixture->SetUserData(this);
}

Boost::~Boost() {
    world_->DestroyBody(body_);
}

void Boost::PrivateUpdate(float dt)
{

}