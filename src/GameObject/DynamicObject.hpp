#pragma once

#include <Box2D/Box2D.h>

#include "GameObject.hpp"

/* 
 * DynamicObject represents an object which affects or is affected by other physics objects.
 * It has a reference to its world, and physics body
 */
class DynamicObject : public GameObject {
public:
    DynamicObject(std::string spritePath, b2World *world);
    virtual ~DynamicObject() = 0;

    //Common update for all DynamicObjects
    void Update(float dt);

    b2Transform GetTransform() const;
    void SetTransform(b2Vec2, float);

    b2Vec2 GetVelocity();
    float GetAngularVelocity();

    void SetVelocity(b2Vec2);
    void SetAngularVelocity(float);

    b2Body *body_;

protected:    
    b2World *world_;
    virtual void PrivateUpdate(float dt);
};