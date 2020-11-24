#include "oilspill.hpp"
#include <iostream>


Oilspill::Oilspill(sf::Int32 id, std::string spritePath, b2World *world): DynamicObject(id, spritePath, world) 
{
    b2PolygonShape pShape;
    pShape.SetAsBox(0.5,0.5);
    shape_ = pShape;
}

Oilspill::~Oilspill() {
    world_->DestroyBody(body_);
}

void Oilspill::PrivateUpdate(float dt)
{

}