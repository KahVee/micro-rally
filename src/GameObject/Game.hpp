#pragma once

#include <vector>
#include <memory>

#include "GameObject.hpp"
#include "DynamicObject.hpp"
#include "Car.hpp"
#include "CarData.hpp"
#include "GameMap.hpp"
#include "RaceLine.hpp"
#include "Box.hpp"

class Game {
public:
    Game(sf::Int32 id, Settings* settings);
    ~Game();

    std::vector<DynamicObject*> GetObjects();
    std::map<sf::Int32, GameObject*> GetObjectMap();
    std::vector<sf::Int32> GetPlayerCarIDs();
    Car *GetPlayerCar();
    GameMap *GetMap();

    void Init();
    void Update(float dt);
    void UpdateObject(sf::Int32 id, b2Transform transform, b2Vec2 velocity, float angularVelocity);
    void UpdateCar(sf::Int32 id, b2Transform transform, b2Vec2 velocity, float angularVelocity, float steeringAngle);

    Car* CreatePlayerCar();
    Car* AddCar(sf::Int32 id);
    void RemoveCar(sf::Int32 id);
    bool ContainsCar(sf::Int32 id);

    sf::Int32 GenerateID();

    float GetFriction(b2Vec2 coords) const;

private:
    sf::Int32 id_;
    Car *playerCar_;
    b2World *world_;
    GameMap *map_;
    Settings* settings_;
    //All objects in the world except player-car and its tires
    std::vector<DynamicObject*> objects_;
    std::map<sf::Int32, GameObject*> objectMap_;
};