#include <vector>
#include <iostream>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Box2D/Box2D.h>

#include "Game.hpp"


Game::Game()
{
    b2Vec2 g = b2Vec2(0,0);
    world_ = new b2World(g);
    map_ = new GameMap(5.0);

    playerCar_ = CreateCar();
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

Car* Game::CreateCar()
{
    Car* car = new Car("../res/f1.png", world_, 2, 4);

    for(auto tire: car->GetTires()) {
        objects_.push_back(tire);
    }
    objects_.push_back(car);
    return car;
}

void Game::AddCar(sf::Int32 id, Car* car)
{
    players_[id] = car;
}

void Game::RemoveCar(sf::Int32 id)
{
    // TODO probably requires some code restructuring
}

void Game::UpdateCar(sf::Int32 id, const b2Transform& transform, const b2Vec2& velocity, float angularVelocity, float steeringAngle)
{
    players_[id]->SetTransform(transform.p, transform.q.GetAngle());
    // TODO set velocity, angularvelocity, steeringAngle
}