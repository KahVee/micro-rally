#include <vector>
#include <iostream>
#include <time.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Box2D/Box2D.h>

#include "Game.hpp"


Game::Game()
{
    b2Vec2 g = b2Vec2(0,0);
    world_ = new b2World(g);
    map_ = new GameMap(5.0);

    playerCar_ = CreatePlayerCar();
    playerCar_->Accelerate(false);
    playerCar_->Brake(false);
    playerCar_->TurnLeft(false);
    playerCar_->TurnRight(false);
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
std::map<sf::Int32, DynamicObject*> Game::GetObjectMap() {
    return objectMap_;
}

Car *Game::GetPlayerCar() {
    return playerCar_;
}

std::vector<sf::Int32> Game::GetPlayerCarIDs() {
    return playerCarIDs_;
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
    for(auto id: playerCarIDs_) {
        objectMap_[id]->Update(dt);
    }
    map_->Update();
    world_->Step(dt, 16, 16);
}

void Game::UpdateObject(sf::Int32 id, b2Transform transform, b2Vec2 velocity, float angularVelocity) {
    DynamicObject *o = objectMap_[id];
    o->SetTransform(transform.p, transform.q.GetAngle());
    o->SetVelocity(velocity);
    o->SetAngularVelocity(angularVelocity);
}

Car* Game::CreatePlayerCar()
{
    //Generates IDs for the car and tires
    for (int i = 0; i < 5; i++)
    {
        playerCarIDs_.push_back(GenerateID());
    }
    Car* car = new Car(playerCarIDs_, "../res/f1.png", world_, 2, 4);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(playerCarIDs_[0], car));
    
    std::vector<Tire*> tires = car->GetTires();
    for(int i = 0; i < 4; i++) {
        objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(playerCarIDs_[i+1], tires[i]));
    }
    return car;
}

Car* Game::AddCar(std::vector<sf::Int32> ids)
{
    Car* car = new Car(ids, "../res/f1.png", world_, 2, 4);   
    objects_.push_back(car);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[0], car));
    std::vector<Tire*> tires = car->GetTires();
    for(int i = 0; i < 4; i++) {
        objects_.push_back(tires[i]);
        objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[i], tires[i]));
    }

    return car;
}

void Game::RemoveCar(sf::Int32 id)
{
    // TODO probably requires some code restructuring
}

sf::Int32 Game::GenerateID() {
    sf::Int32 r = std::rand();
    for(auto o: objects_) {
        if(r == o->GetID()) {
            r = GenerateID();
            break;
        }
    }
    return r;
}