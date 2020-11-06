#include <vector>
#include <iostream>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <box2d/b2_math.h>

#include "Game.hpp"


Game::Game() {
    objects_ = std::vector<DynamicObject*>();
    b2Vec2 g = b2Vec2(0,0);
    world_ = new b2World(g);
    map_ = new GameMap(5.0);

    playerCar_ = new Car("../res/f1.png", world_, 2, 4);

    for(auto tire: playerCar_->GetTires()) {
        objects_.push_back((DynamicObject*)tire);
    }
    objects_.push_back((DynamicObject*)playerCar_);
    // Load texture for menu background
    if (!mapTexture_.loadFromFile("../res/Lemon.jpg"))
    {
        std::cout << "texture load error" << std::endl;
    }
    // Set sprite
    mapSprite_.setTexture(mapTexture_);
}

Game::~Game() {
    for(auto o: objects_) {
        delete o;
    }
    delete world_;
    delete map_;
}

std::vector<DynamicObject*> Game::GetObjects(){
    return objects_;
}

Car *Game::GetPlayerCar() {
    return playerCar_;
}

GameMap *Game::GetMap() {
    return map_;
}


void Game::Init() {

}

void Game::Update(float dt) {
    for(auto object: objects_) {
        object->Update(dt);
    }
    map_->Update();
    world_->Step(dt, 16, 16);
}