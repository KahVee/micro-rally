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

    playerCar_ = new Car("../res/f1.png", world_, 2, 4);

    for(auto tire: playerCar_->GetTires()) {
        objects_.push_back((DynamicObject*)tire);
    }
    objects_.push_back((DynamicObject*)playerCar_);
}

Game::~Game() {
    for(auto o: objects_) {
        delete o;
    }
    delete world_;
}

std::vector<DynamicObject*> Game::GetObjects(){
    return objects_;
}

Car *Game::GetPlayerCar() {
    return playerCar_;
}

void Game::Init() {

}

void Game::Update(float dt) {
    for(auto object: objects_) {
        object->Update(dt);
    }
    world_->Step(dt, 16, 16);
}
