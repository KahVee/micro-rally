#pragma once

#include "GameObject.hpp"
#include "DynamicObject.hpp"
#include "MapTile.hpp"
#include "GameMapDrawable.hpp"
#include <Box2D/Box2D.h>
#include <vector>
#include <map>
#include <string>
#include "RaceLine.hpp"
#include "../settings/Settings.hpp"

class GameMap : public GameObject {
public:
    GameMap(sf::Int32 id, Settings* settings);
    virtual ~GameMap();

    b2Transform GetTransform() const;
    void SetTransform(b2Vec2, float);

    void Update();

    const float GetFriction(b2Vec2) const;
    const float GetRollingResistance(b2Vec2) const;
    void LoadMapFile(const std::string& filepath, b2World* world, std::map<sf::Int32, GameObject*> *objectMap, 
                std::vector<DynamicObject*> *objects, 
                std::vector<DynamicObject*> *networkedObjects);
    GameMapDrawable GetMapDrawable() const;
    int GetWidth() const;
    int GetHeight() const;
    float GetTileSize() const;
    int GetNumberOfRaceLines() const;
    const b2Transform GetStartingPosition(int i) const;

private:
    GameMapDrawable mapDrawable_;
    int width_;
    int height_;
    float tileSize_ = 1.0; // In respect to simluation coordinates
    std::map<char, MapTile*> tileTypes_;
    std::vector<MapTile*> map_;
    MapTile* backgroundTileType_;
    std::vector<RaceLine*> raceLines_;
    std::vector<b2Transform*> startPoints_;
    Settings* settings_;
};