#include <vector>
#include <iostream>
#include <time.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Box2D/Box2D.h>

#include "Game.hpp"


Game::Game(sf::Int32 id): id_(id) {
    b2Vec2 g = b2Vec2(0,0);
    world_ = new b2World(g);
    map_ = new GameMap(5.0);

    playerCar_ = CreatePlayerCar();
    playerCar_->Accelerate(false);
    playerCar_->Brake(false);
    playerCar_->TurnLeft(false);
    playerCar_->TurnRight(false);
    Box *box = new Box(GenerateID(), "../res/box.png", world_);
    box->SetTransform(b2Vec2(0,15), 0.0);
    objects_.push_back(box);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(box->GetID(), box));
    Tirestack *tirestack = new Tirestack(GenerateID(), "../res/tirestack.png", world_);
    tirestack->SetTransform(b2Vec2(0,10), 0.0);
    objects_.push_back(tirestack);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(tirestack->GetID(), tirestack) );
    Oilspill *oilspill = new Oilspill(GenerateID(), "../res/oilspill.png", world_);
    oilspill->SetTransform(b2Vec2(0,11), 0.0);
    objects_.push_back(oilspill);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(oilspill->GetID(), oilspill) );
}

Game::~Game() {
    for(auto o: objects_) {
        if(o != nullptr)
            delete o;
    }
    delete playerCar_;
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
    map_->Update();
    world_->Step(dt, 3, 8);
}

void Game::UpdateObject(sf::Int32 id, b2Transform transform, b2Vec2 velocity, float angularVelocity) {
    DynamicObject *o = objectMap_[id];
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
    std::vector<std::pair<float, float>> tirePositions = { {-0.8, 1.1 }, {0.8, 1.1}, {-0.8, -1.7}, {0.8, -1.7}};
    Car* car = new Car(ids, "../res/f1.png", world_, 2, 4, tirePositions);
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
    std::vector<std::pair<float, float>> tirePositions = { {-0.8, 1.1 }, {0.8, 1.1}, {-0.8, -1.7}, {0.8, -1.7}};
    Car* car = new Car(ids, "../res/f1.png", world_, 2, 4, tirePositions);   
    objects_.push_back(car);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[0], car));
    std::vector<Tire*> tires = car->GetTires();
    for(int i = 0; i < 4; i++) {
        objects_.push_back(tires[i]);
        objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[i], tires[i]));
    }
    car->isLocalPlayer_ = false;
    return car;
}

void Game::RemoveCar(sf::Int32 id)
{
    //TODO add exception handling
    Car *carToRemove = (Car*)objectMap_.at(id);
    objectMap_.erase(id);
    for(auto t: carToRemove->GetTires()) {
        objectMap_.erase(t->GetID());
    }
    objects_.erase(std::remove(objects_.begin(), objects_.end(), carToRemove), objects_.end());
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
