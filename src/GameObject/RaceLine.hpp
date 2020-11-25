#pragma once

#include "GameObject.hpp"
#include <Box2D/Box2D.h>
#include <string>

class RaceLine : public GameObject {
public:
    RaceLine(double length, double thickness, b2Vec2 pos, float rotation, b2World *world, sf::Int32 id);

    b2Transform GetTransform() const  {return transform_;};
    void SetTransform(b2Vec2, float) {};

    b2Body *body_;
private:
    
};