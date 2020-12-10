#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "../settings/Settings.hpp"
#include "GameObject.hpp"

/* 
 * DynamicObject represents an object which affects or is affected by other physics objects.
 * It has a reference to its world, and physics body
 */
class DynamicObject : public GameObject {
public:
    DynamicObject(sf::Int32 id, std::string spritePath, b2World *world, sf::RenderWindow* window);
    virtual ~DynamicObject() = default;

    //Common update for all DynamicObjects
    void Update(float dt);

    void SetState(b2Transform transform, b2Vec2 velocity, float angularVelocity);

    b2Transform GetTransform() const;
    virtual void SetTransform(b2Vec2, float);

    b2Vec2 GetVelocity();
    void SetVelocity(b2Vec2);

    float GetAngularVelocity();    
    void SetAngularVelocity(float);

    b2Body *body_;

    void UpdateFriction(float friction);
    void UpdateRollingRresistance(float rollingResistance);

protected:    
    b2World *world_;
    virtual void PrivateUpdate(float dt);
    float frictionMultiplier_ = 1;
    sf::RenderWindow* window_;
    float rollingResistance_ = 1;
};