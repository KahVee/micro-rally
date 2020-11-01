#include "DynamicObject.hpp"

DynamicObject::DynamicObject(sf::Sprite sprite, b2World *world): world_(world) {
    b2BodyDef bDef;
    bDef.type = b2_dynamicBody;
    body_ = world->CreateBody(&bDef);
    sprite_ = sprite;
}

DynamicObject::~DynamicObject() {
    
}

//TODO: fix usage of transform_, maybe make it a reference?
b2Transform DynamicObject::GetTransform() const {
    return body_->GetTransform();
}

void DynamicObject::SetTransform(b2Vec2 t, float a) {
    body_->SetTransform(t, a);
}