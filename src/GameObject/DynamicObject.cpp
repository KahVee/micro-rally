#include <iostream>

#include "DynamicObject.hpp"
#include "../constants.hpp"

DynamicObject::DynamicObject(sf::Int32 id, std::string spritePath, b2World *world, Settings* settings): GameObject(id), world_(world), settings_(settings) {
    b2BodyDef bDef;
    bDef.type = b2_dynamicBody;
    body_ = world->CreateBody(&bDef);
    LoadSprite(spritePath);
}

void DynamicObject::Update(float dt) {
    b2Vec2 worldPos = body_->GetTransform().p;
    float worldRot = body_->GetTransform().q.GetAngle();
    sprite_.setPosition(sf::Vector2f(worldPos.x, settings_->GetVideoMode().height-worldPos.y));
    sprite_.setRotation(-worldRot*RAD_TO_DEG);
    PrivateUpdate(dt);
}

void DynamicObject::PrivateUpdate(float dt) {

}

void DynamicObject::SetState(b2Transform transform, b2Vec2 velocity, float angularVelocity) {
    SetTransform(transform.p, transform.q.GetAngle());
    SetVelocity(velocity);
    SetAngularVelocity(angularVelocity);
}

//TODO: fix usage of transform_, maybe make it a reference?
b2Transform DynamicObject::GetTransform() const {
    return body_->GetTransform();
}

void DynamicObject::SetTransform(b2Vec2 t, float a) {
    body_->SetTransform(t, a);
}

b2Vec2 DynamicObject::GetVelocity() {
    return body_->GetLinearVelocity();
}

void DynamicObject::SetVelocity(b2Vec2 v) {
    body_->SetLinearVelocity(v);
}

float DynamicObject::GetAngularVelocity() {
    return body_->GetAngularVelocity();
}

void DynamicObject::SetAngularVelocity(float a) {
    body_->SetAngularVelocity(a);
}

void DynamicObject::UpdateFriction(float friction)
{
    frictionMultiplier_ = friction;
}