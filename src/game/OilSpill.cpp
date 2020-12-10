#include "OilSpill.hpp"
#include <iostream>


Oilspill::Oilspill(sf::Int32 id, std::string spritePath, b2World *world, sf::RenderWindow* window): DynamicObject(id, spritePath, world, window) 
{
    b2PolygonShape pShape;
    float width = 2.0f;
    float height = 2.0f;
    pShape.SetAsBox(width/2,height/2);
    shape_ = pShape;

    b2FixtureDef fDef;
    fDef.isSensor = true;
    fDef.shape = &shape_;
    b2Fixture* fixture = body_->CreateFixture(&fDef);
    sprite_.setScale(width / sprite_.getLocalBounds().width, height / sprite_.getLocalBounds().height);

    fixture->SetUserData(this);
}

Oilspill::~Oilspill() {
    world_->DestroyBody(body_);
}

void Oilspill::PrivateUpdate(float dt)
{

}