#include "RaceLine.hpp"

RaceLine::RaceLine(double length, double thickness, b2Vec2 pos, float rotation, b2World *world, sf::Int32 id) : GameObject(id) {
    // Create body
    b2BodyDef bDef;
    bDef.type = b2_staticBody;
    body_ = world->CreateBody(&bDef);
    
    // Create shape
    b2PolygonShape shape;
    shape.SetAsBox(thickness/2, length/2);

    // Create fixture
    b2FixtureDef fDef;
    fDef.isSensor = true;
    fDef.shape = &shape;
    fDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    b2Fixture* fixture = body_->CreateFixture(&fDef);

    // Save GameObject to body's userdata. This is needed in collision events.
    // fixture->SetUserData(this);
    
    // Set position in Box2d world
    body_->SetTransform(pos, rotation);
}