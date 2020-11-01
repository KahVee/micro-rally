#include <SFML/Graphics/Sprite.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_math.h>
#include <box2d/b2_polygon_shape.h>

#include "Tire.hpp"

Tire::Tire(sf::Sprite sprite, b2World *world): DynamicObject(sprite, world) {
    //car_ = car;
    b2PolygonShape pShape;
    pShape.SetAsBox(0.5f, 1.5f);
    body_->CreateFixture(&pShape, 1);
}

Tire::~Tire() {
    world_->DestroyBody(body_);
}

void Tire::UpdateFriction() {
    b2Vec2 lateralDirection = body_->GetWorldVector(b2Vec2(1,0));
    b2Vec2 forwardDirection = body_->GetWorldVector(b2Vec2(0,1));
    b2Vec2 lateralVelocity = b2Dot(lateralDirection, body_->GetLinearVelocity()) * lateralDirection;
    b2Vec2 forwardVelocity = b2Dot(forwardDirection, body_->GetLinearVelocity()) * forwardDirection;

    //TODO: Test ApplyForce() too
    //Stops tire from sliding sideways
    body_->ApplyLinearImpulse(body_->GetMass() * -lateralVelocity, body_->GetWorldCenter(), true);
    //Stops tire from spinning around
    body_->ApplyAngularImpulse( AngularVelocityMultiplier * body_->GetInertia() * -body_->GetAngularVelocity(), true);
}

void Tire::DebugApplyForce() {
    body_->ApplyLinearImpulse(b2Vec2(15,10), b2Vec2(0,0), true);
}