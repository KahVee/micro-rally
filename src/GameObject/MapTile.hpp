#pragma once

#include "GameObject.hpp"
#include <Box2D/Box2D.h>
#include <string>

class MapTile {
public:
    MapTile(std::string name, float friction, int textureLocationX, int textureLocationY) 
        : name_(name), friction_(friction), textureLocationX_(textureLocationX), textureLocationY_(textureLocationY) {};
    const float GetFriction() const;
    const std::string GetName() const;
    const int textureLocationX_;
    const int textureLocationY_;
private:
    float friction_;
    std::string name_;
};