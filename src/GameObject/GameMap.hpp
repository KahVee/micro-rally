#pragma once

#include "GameObject.hpp"
#include "MapTile.hpp"
#include "GameMapDrawable.hpp"
#include <box2d/b2_body.h>
#include <vector>
#include <map>
#include <string>

class GameMap : public GameObject {
public:
    GameMap(float tileSize);
    virtual ~GameMap();

    b2Transform GetTransform() const;
    void SetTransform(b2Vec2, float);

    void Update();

    const float GetFriction(b2Vec2) const;
    void LoadMapFile(const std::string&);
    GameMapDrawable GetMapDrawable() const;
    int GetWidth() const;
    int GetHeight() const;
    float GetTileSize() const;
private:
    GameMapDrawable mapDrawable_;
    int width_;
    int height_;
    float tileSize_ = 1.0; // In respect to simluation coordinates
    std::map<char, MapTile*> tileTypes_;
    std::vector<MapTile*> map_;
};