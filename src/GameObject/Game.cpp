#include <vector>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Box2D/Box2D.h>

#include "Game.hpp"
#include "ContactListener.cpp"
#include "RaceLine.hpp"

ContactListener gameContactListener;


Game::Game(sf::Int32 id, Settings* settings): id_(id), settings_(settings) {
    b2Vec2 g = b2Vec2(0,0);
    world_ = new b2World(g);

    // Set world contact event listener
    world_->SetContactListener(&gameContactListener);

    map_ = new GameMap(5.0, -2, settings);
    map_->LoadMapFile("../res/maps/test_map_file.json", world_);

    playerCar_ = CreatePlayerCar();
    playerCar_->Accelerate(false);
    playerCar_->Brake(false);
    playerCar_->TurnLeft(false);
    playerCar_->TurnRight(false);
    playerCar_->SetTransform(b2Vec2(50, 50), 0);

    Box *box = new Box(GenerateID(), "../res/smallcrate.png", world_, settings_);
    box->SetTransform(b2Vec2(20,30), 0.0);
    objects_.push_back(box);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(box->GetID(), box));

}


// TODO: Maybe change destruction from objects_ to objectMap_?
Game::~Game() {
    for(auto o: objects_) {
        delete o;
    }
    for(Tire *t: playerCar_->GetTires()) {
        delete t;
    }
    delete playerCar_;
    delete world_;
    delete map_;
}

std::vector<DynamicObject*> Game::GetObjects(){
    return objects_;
}
std::map<sf::Int32, GameObject*> Game::GetObjectMap() {
    return objectMap_;
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
        object->UpdateFriction(map_->GetFriction(object->GetTransform().p) );
    }
    playerCar_->Update(dt);
    for(Tire *t : playerCar_->GetTires()) {
        t->UpdateFriction(map_->GetFriction(t->GetTransform().p));
    }
    map_->Update();
    world_->Step(dt, 3, 8);
}

/*
* BE CAREFUL WHEN CALLING THIS (only call on objects which are dynamicobjects)
*/  
void Game::UpdateObject(sf::Int32 id, b2Transform transform, b2Vec2 velocity, float angularVelocity) {
    DynamicObject *o = static_cast<DynamicObject*>(objectMap_[id]);
    o->SetState(transform, velocity, angularVelocity);
}

void Game::UpdateCar(sf::Int32 id, b2Transform transform, b2Vec2 velocity, float angularVelocity, float steeringAngle) {
    Car *car = (Car*)objectMap_[id];
    car->SetState(transform, velocity, angularVelocity, steeringAngle);
}

Car* Game::CreatePlayerCar()
{
    std::vector<sf::Int32> ids;
    ids.push_back(id_);
    //Generates IDs for the car and tires
    for (int i = 0; i < 4; i++)
    {
        ids.push_back(GenerateID());
    }
    Car* car = new Car(ids, world_, TRUCK, settings_);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[0], car));
    
    std::vector<Tire*> tires = car->GetTires();
    for(int i = 0; i < 4; i++) {
        objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[i+1], tires[i]));
    }
    car->isLocalPlayer_ = true;
    return car;
}

Car* Game::AddCar(sf::Int32 id)
{
    std::vector<sf::Int32> ids;
    ids.push_back(id);
    for (int i = 0; i < 4; i++)
    {
        ids.push_back(GenerateID());
    }
    Car* car = new Car(ids, world_, TRUCK, settings_);  
    objects_.push_back(car);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[0], car));
    std::vector<Tire*> tires = car->GetTires();
    for(int i = 0; i < 4; i++) {
        objects_.push_back(tires[i]);
        objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[i+1], tires[i]));
    }
    car->isLocalPlayer_ = false;
    return car;
}

void Game::RemoveCar(sf::Int32 id)
{
    //TODO add exception handling
    Car *carToRemove = (Car*)objectMap_.at(id);
    objectMap_.erase(id);
    for(auto t: carToRemove->GetTires())
    {
        objectMap_.erase(t->GetID());
    }
    // Car
    objects_.erase(std::remove(objects_.begin(), objects_.end(), carToRemove), objects_.end());
    // Tires
    for(auto t: carToRemove->GetTires())
    {
        objects_.erase(std::remove(objects_.begin(), objects_.end(), t), objects_.end());
    }
    for(Tire *t: carToRemove->GetTires()) {
        delete t;
    }
    delete carToRemove;
}

float Game::GetFriction(b2Vec2 coords) const
{
    return map_->GetFriction(coords);
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
