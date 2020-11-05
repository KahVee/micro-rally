#include <iostream>

#include "DynamicObject.hpp"
#include "../constants.hpp"

DynamicObject::DynamicObject(std::string spritePath, b2World *world): world_(world) {
    b2BodyDef bDef;
    bDef.type = b2_dynamicBody;
    body_ = world->CreateBody(&bDef);
    LoadSprite(spritePath);
}

DynamicObject::~DynamicObject() {
    
}

void DynamicObject::Update(float dt) {
    b2Vec2 worldPos = body_->GetTransform().p;
    float worldRot = body_->GetTransform().q.GetAngle();
    sprite_.setPosition(sf::Vector2f(PIXELS_PER_METER*worldPos.x, WINDOW_HEIGHT-(PIXELS_PER_METER*worldPos.y)));
    sprite_.setRotation(-worldRot*RAD_TO_DEG);
    PrivateUpdate(dt);
}

void DynamicObject::PrivateUpdate(float dt) {

}

//TODO: fix usage of transform_, maybe make it a reference?
b2Transform DynamicObject::GetTransform() const {
    return body_->GetTransform();
}

void DynamicObject::SetTransform(b2Vec2 t, float a) {
    body_->SetTransform(t, a);
}