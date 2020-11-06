#pragma once

#include <vector>
#include <memory>

#include "GameObject.hpp"
#include "DynamicObject.hpp"
#include "Car.hpp"

class Game {
    public:
        Game();
        ~Game();

        std::vector<DynamicObject*> GetObjects();
        Car *GetPlayerCar();

        void Init();
        void Update(float dt);
        sf::Sprite GetMapSprite();

    private:
        Car *playerCar_;
        b2World *world_;
        //Map map_;
        std::vector<DynamicObject*> objects_;
        sf::Texture mapTexture_;
        sf::Sprite mapSprite_;
};