#pragma once

#include "GameObject.hpp"
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
    GameMap(float tileSize, sf::Int32 id, Settings* settings);
    virtual ~GameMap();

    b2Transform GetTransform() const;
    void SetTransform(b2Vec2, float);

    void Update();

    const float GetFriction(b2Vec2) const;
    void LoadMapFile(const std::string&, b2World* world);
    GameMapDrawable GetMapDrawable() const;
    int GetWidth() const;
    int GetHeight() const;
    float GetTileSize() const;
    int GetNumberOfRaceLines() const;

private:
    GameMapDrawable mapDrawable_;
    int width_;
    int height_;
    float tileSize_ = 1.0; // In respect to simluation coordinates
    std::map<char, MapTile*> tileTypes_;
    std::vector<MapTile*> map_;
    std::vector<RaceLine*> raceLines_;
    Settings* settings_;
};