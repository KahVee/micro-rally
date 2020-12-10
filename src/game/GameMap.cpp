#include "GameMap.hpp"
#include <map>
#include "../lib/json.hpp"
#include <fstream>
#include <iostream>
#include "../constants.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include "RaceLine.hpp"
#include "OilSpill.hpp"
#include "TireStack.hpp"
#include "Boost.hpp"
#include "Box.hpp"
#include <map>

using json = nlohmann::json;

GameMap::GameMap(sf::Int32 id, sf::RenderWindow* window) : GameObject(id), window_(window) {
    
}

GameMap::~GameMap() {
    for (auto const& it : this->tileTypes_) {
        delete it.second;
    }

    for (auto const& raceLine : this->raceLines_) {
        delete raceLine;
    }

    for (auto const& startPoint : this->startPoints_) {
        delete startPoint;
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
        return backgroundTileType_->GetFriction();
    }
}

const float GameMap::GetRollingResistance(b2Vec2 v) const {
    int x = static_cast<int>(v.x/tileSize_);
    int y = static_cast<int>(v.y/tileSize_);
    if (x >= 0 && x < width_ && y >= 0 && y< height_) {
        int row = height_ - y - 1;
        int column = x;
        int mapIndex = row*width_ + column;
        return map_[mapIndex]->GetRollingResistance();
    } else {
        // If outside map
        return backgroundTileType_->GetRollingResistance();
    }
}

/* 
 * Loads the config file for tiles and loads a saved map file.
 */
void GameMap::LoadMapFile(const std::string& mapName, b2World* world, std::map<sf::Int32, GameObject*> *objectMap, 
                        std::vector<DynamicObject*> *objects, 
                        std::vector<DynamicObject*> *networkedObjects) {
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
            tile_stats["rollingResistance"].get<float>(),
            textureIndex%MAP_TEXTURE_TILES * (MAP_TEXTURE_MAP_SIZE/MAP_TEXTURE_TILES),  // Column in texture map
            textureIndex/MAP_TEXTURE_TILES * (MAP_TEXTURE_MAP_SIZE/MAP_TEXTURE_TILES)); // Row in texture map
        
        tileTypes_[tile_stats["fileChar"].get<std::string>()[0]] = tile;
    }

    // Read json file (map file)
    std::ifstream mapFile("../res/maps.json");
    json mapJson;
    mapFile >> mapJson;
    mapFile.close();
    mapJson = mapJson[mapName];
    
    width_ = mapJson["width"].get<int>();
    height_ = mapJson["height"].get<int>();
    tileSize_ = mapJson["tileSize"].get<int>();
    backgroundTileType_ = tileTypes_[mapJson["bgTileType"].get<std::string>()[0]];

    std::string tileLayoutString;
    for (int i = 0; i < height_; i++) {
        tileLayoutString.append(mapJson["tileLayout"][i].get<std::string>());
    }

    // Push references of appropriate MapTile types to map_
    for (auto tileChar : tileLayoutString) {
        map_.push_back(tileTypes_[tileChar]);
    }
    // Load map textures
    mapDrawable_.load("../res/tiles.png", tileSize_, map_, width_, height_, backgroundTileType_);
    // Set map position, so that left bottom coordinate is at (0,0) in box2d
    mapDrawable_.setPosition(sf::Vector2f(0, window_->getSize().y - tileSize_*height_));

    // Load finish line and checkpoints
    for (int i = 0; i < mapJson["raceLines"].size(); i++) {
        float xPos = mapJson["raceLines"][i]["mapX"].get<float>()*tileSize_;
        float yPos = mapJson["raceLines"][i]["mapY"].get<float>()*tileSize_;
        float rotation = mapJson["raceLines"][i]["rotation"].get<float>()*DEG_TO_RAD;
        float length = mapJson["raceLines"][i]["length"].get<float>()*tileSize_;
        b2Vec2 pos = b2Vec2(xPos, yPos);
        raceLines_.push_back(new RaceLine(length, tileSize_, pos, rotation, world, -100 -i)); //TODO: ID setting
    }

    // Load starting positions for players
    for (int i = 0; i < mapJson["startPoints"].size(); i++) {
        float xPos = mapJson["startPoints"][i][0].get<float>()*tileSize_;
        float yPos = mapJson["startPoints"][i][1].get<float>()*tileSize_;
        float rotation = mapJson["startPoints"][i][2].get<float>()*DEG_TO_RAD;
        b2Transform *position = new b2Transform(b2Vec2(xPos,yPos), b2Rot(rotation));
        startPoints_.push_back(position);
    }

    // Load different objects
    // Oilspills
    for (int i = 0; i < mapJson["oilSpills"].size(); i++) {
        float xPos = mapJson["oilSpills"][i][0].get<float>()*tileSize_;
        float yPos = mapJson["oilSpills"][i][1].get<float>()*tileSize_;
        float size = mapJson["oilSpills"][i][2].get<float>()*tileSize_;
        float rotation = mapJson["oilSpills"][i][3].get<float>()*DEG_TO_RAD;

        Oilspill *oilSpill = new Oilspill(400 + i, "../res/oilspill.png", world, window_);
        oilSpill->SetTransform(b2Vec2(xPos,yPos), rand());
        objects->push_back(oilSpill);
        objectMap->insert(std::pair<sf::Int32, DynamicObject*>(oilSpill->GetID(), oilSpill) );
    }

    // Boosts
    for (int i = 0; i < mapJson["boosts"].size(); i++) {
        float xPos = mapJson["boosts"][i][0].get<float>()*tileSize_;
        float yPos = mapJson["boosts"][i][1].get<float>()*tileSize_;
        float power = mapJson["boosts"][i][2].get<float>()*tileSize_;

        std::string path = rand() % 50 ? (rand() % 2 ? "../res/boost1.png" : "../res/boost2.png") : "../res/boost.png";
        Boost *boost = new Boost(300 + i, path, world, window_, power);
        boost->SetTransform(b2Vec2(xPos,yPos), 0);
        objects->push_back(boost);
        objectMap->insert(std::pair<sf::Int32, DynamicObject*>(boost->GetID(), boost) );
    }

    // Boxes
    for (int i = 0; i < mapJson["boxes"].size(); i++) {
        float xPos = mapJson["boxes"][i][0].get<float>()*tileSize_;
        float yPos = mapJson["boxes"][i][1].get<float>()*tileSize_;
        float size = mapJson["boxes"][i][2].get<float>()*tileSize_;
        float rotation = mapJson["boxes"][i][3].get<float>()*DEG_TO_RAD;

        std::string path = size >= 2 ? "../res/bigcrate.png" : "../res/smallcrate.png";
        Box *box = new Box(500+i, path, world, window_, size);
        box->SetTransform(b2Vec2(xPos,yPos), rotation);
        objects->push_back(box);
        networkedObjects->push_back(box);
        objectMap->insert(std::pair<sf::Int32, DynamicObject*>(box->GetID(), box));
    }

    // Tirestacks
    for (int i = 0; i < mapJson["tireStacks"].size(); i++) {
        float xPos = mapJson["tireStacks"][i][0].get<float>()*tileSize_;
        float yPos = mapJson["tireStacks"][i][1].get<float>()*tileSize_;

        float xRandomOffset = rand() % 100 * 0.00015;
        float yRandomOffset = rand() % 100 * 0.00015;

        TireStack *tireStack = new TireStack(600+i, "../res/tirestack.png", world, window_);
        tireStack->SetTransform(b2Vec2(xPos + xRandomOffset, yPos + yRandomOffset), 0.0);
        objects->push_back(tireStack);
        objectMap->insert(std::pair<sf::Int32, DynamicObject*>(tireStack->GetID(), tireStack) );
    }
}

void GameMap::Update() {

}

const b2Transform GameMap::GetStartingPosition(int i) const {
    return *(startPoints_[i]);
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

int GameMap::GetNumberOfRaceLines() const {
    return raceLines_.size();
}