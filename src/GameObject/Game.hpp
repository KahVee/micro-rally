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

private:
    Car *playerCar_;
    b2World *world_;
    GameMap *map_;
    std::vector<DynamicObject*> objects_;
};