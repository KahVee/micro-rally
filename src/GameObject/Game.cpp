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


Game::Game(sf::Int32 id, Settings* settings, int laps, const std::string &playerCarType, std::string mapPath)
    : id_(id), settings_(settings) {
    b2Vec2 g = b2Vec2(0,0);
    world_ = new b2World(g);

    // Set world contact event listener
    gameContactListener.SetSettings(settings);
    world_->SetContactListener(&gameContactListener);

    map_ = new GameMap(1.6, -2, settings);
    map_->LoadMapFile(mapPath, world_);

    playerCar_ = CreatePlayerCar(playerCarType);
    playerCar_->Accelerate(false);
    playerCar_->Brake(false);
    playerCar_->TurnLeft(false);
    playerCar_->TurnRight(false);
    playerCar_->SetTransform(b2Vec2(50, 50), 0);

    Box *box = new Box(GenerateID(), "../res/smallcrate.png", world_, settings_);
    box->SetTransform(b2Vec2(20,30), 0.0);
    objects_.push_back(box);
    networkedObjects_.push_back(box);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(box->GetID(), box));
    Tirestack *tirestack = new Tirestack(GenerateID(), "../res/tirestack.png", world_, settings_);
    tirestack->SetTransform(b2Vec2(0,10), 0.0);
    objects_.push_back(tirestack);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(tirestack->GetID(), tirestack) );
    Oilspill *oilspill = new Oilspill(GenerateID(), "../res/oilspill.png", world_, settings_);
    oilspill->SetTransform(b2Vec2(0,11), 0.0);
    objects_.push_back(oilspill);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(oilspill->GetID(), oilspill) );
    Boost *boost = new Boost(GenerateID(), "../res/boost.png", world_,settings_);
    boost->SetTransform(b2Vec2(0,5), 0.0);
    objects_.push_back(boost);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(boost->GetID(), boost) );
}


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
std::vector<DynamicObject*> Game::GetNetworkedObjects() {
    return networkedObjects_;
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

Car* Game::CreatePlayerCar(const std::string &carType)
{
    std::vector<sf::Int32> ids;
    ids.push_back(id_);
    //Generates IDs for the car and tires
    for (int i = 0; i < 4; i++)
    {
        ids.push_back(GenerateID());
    }
    Car* car = new Car(ids, world_, settings_->GetCarData(carType), settings_);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[0], car));
    
    std::vector<Tire*> tires = car->GetTires();
    for(int i = 0; i < 4; i++) {
        objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[i+1], tires[i]));
    }
    car->isLocalPlayer_ = true;
    return car;
}

Car* Game::AddCar(sf::Int32 id, const std::string &carType)
{
    std::vector<sf::Int32> ids;
    ids.push_back(id);
    for (int i = 0; i < 4; i++)
    {
        ids.push_back(GenerateID());
    }
    Car* car = new Car(ids, world_, settings_->GetCarData(carType), settings_);  
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

bool Game::ContainsCar(sf::Int32 id)
{
    return objectMap_.find(id) != objectMap_.end();
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
