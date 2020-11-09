#pragma once

#include <vector>
#include <memory>

#include "GameObject.hpp"
#include "DynamicObject.hpp"
#include "Car.hpp"
#include "GameMap.hpp"

class Game {
public:
    Game();
    ~Game();

    std::vector<DynamicObject*> GetObjects();
    Car *GetPlayerCar();
    GameMap *GetMap();

    void Init();
    void Update(float dt);

    Car* CreateCar();
    void AddCar(sf::Int32 id, Car* car);
    void RemoveCar(sf::Int32 id);
    void UpdateCar(sf::Int32 id, const b2Transform& transform, const b2Vec2& velocity, float angularVelocity, float steeringAngle);
private:
    Car *playerCar_;
    b2World *world_;
    GameMap *map_;
    std::vector<DynamicObject*> objects_;
    std::map<sf::Int32, Car*> players_;
};