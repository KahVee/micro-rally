#include "GameMap.hpp"
#include <map>
#include "../lib/json.hpp"
#include <fstream>
#include <iostream>
#include "../constants.hpp"

using json = nlohmann::json;

GameMap::GameMap(float tileSize) : tileSize_(tileSize) {
    LoadMapFile("../res/maps/test_map_file.json");
}

GameMap::~GameMap() {
    for (auto const& it : this->tileTypes_) {
        delete it.second;
    }
}

/* 
 * Gets the friction value of a tile in the map.
 * The reference frame is always in the same orientation as the map.
 */
const float GameMap::GetFriction(b2Vec2 v) const {
    int x = static_cast<int>(v.x/tileSize_);
    int y = static_cast<int>(v.y/tileSize_);
    if (x >= 0 && x < width_ && y >= 0 && y< height_) {
        int row = height_ - y - 1;
        int column = x;
        int mapIndex = row*width_ + column;
        return map_[mapIndex]->GetFriction();
    } else {
        // If outside map
        return 1.0;
    }

}

/* 
 * Loads the config file for tiles and loads a saved map file.
 */
void GameMap::LoadMapFile(const std::string& filepath) {
    // Read json file (tile types)
    std::ifstream tilesFile("../config/map_tile_config.json");
    json tileTypeJson;
    tilesFile >> tileTypeJson;
    tilesFile.close();

    // TODO: Add exception handling
    // Parse tiletypes and add them to tileTypes_
    for (int i = 0; i < tileTypeJson["tileTypes"].size(); i++) {
        auto tile_stats = tileTypeJson["tileTypes"][i];

        int textureIndex = tile_stats["texture"].get<int>();
        auto tile = new MapTile(tile_stats["name"].get<std::string>(), tile_stats["friction"].get<float>(),
            textureIndex%MAP_TEXTURE_TILES * (MAP_TEXTURE_MAP_SIZE/MAP_TEXTURE_TILES),  // Column in texture map
            textureIndex/MAP_TEXTURE_TILES * (MAP_TEXTURE_MAP_SIZE/MAP_TEXTURE_TILES)); // Row in texture map
        
        tileTypes_[tile_stats["fileChar"].get<std::string>()[0]] = tile;
    }

    // Read json file (map file)
    std::ifstream mapFile(filepath);
    json mapJson;
    mapFile >> mapJson;
    mapFile.close();

    width_ = mapJson["width"].get<int>();
    height_ = mapJson["height"].get<int>();

    std::string tileLayoutString;
    for (int i = 0; i < height_; i++) {
        tileLayoutString.append(mapJson["tileLayout"][i].get<std::string>());
    }

    // Push references of appropriate MapTile types to map_
    for (auto tileChar : tileLayoutString) {
        map_.push_back(tileTypes_[tileChar]);
    }

    mapDrawable_.load("../res/mc_texture.png",
        PIXELS_PER_METER*tileSize_, map_, width_, height_);
    std::cout << "Map loaded!" << std::endl;
}

void GameMap::Update() {
    // This currently works without these lines. Only left here just in case
    // b2Vec2 worldPos = transform_.p;
    // float worldRot = transform_.q.GetAngle();
    // mapDrawable_.setPosition(sf::Vector2f(PIXELS_PER_METER*worldPos.x, 
    //    WINDOW_HEIGHT- PIXELS_PER_METER*(worldPos.y + tileSize_*height_)));
    // mapDrawable_.setRotation(-worldRot*RAD_TO_DEG);
}

b2Transform GameMap::GetTransform() const {
    return transform_;
}

void GameMap::SetTransform(b2Vec2 t, float a) {
    transform_.Set(t, a);
}

GameMapDrawable GameMap::GetMapDrawable() const
{
    return mapDrawable_;
}

int GameMap::GetWidth() const
{
    return width_;
}

int GameMap::GetHeight() const
{
    return height_;
}

float GameMap::GetTileSize() const
{
    return tileSize_;
}