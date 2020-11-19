#include "box.hpp"

Box::Box(sf::Int32 id, std::string spritePath, b2World *world): DynamicObject(id, spritePath, world) 
{
    b2PolygonShape pShape;
    pShape.SetAsBox(1,1);
    shape_ = pShape;
    b2FixtureDef fDef;
    fDef.shape = &shape_;
    body_->CreateFixture(&fDef_);

}
Box::~Box() {
    world_->DestroyBody(body_);
}

