#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include "GameObject.hpp"

/* 
 * DynamicObject represents an object which affects or is affected by other physics objects.
 * It has a reference to its world, and physics body
 */
class DynamicObject : public GameObject {
    public:
    DynamicObject(sf::Sprite sprite, b2World *world);
    virtual ~DynamicObject() = 0;

    virtual b2Transform GetTransform() const;
    virtual void SetTransform(b2Vec2, float);

    protected:
    b2World *world_;
    b2Body *body_;
};