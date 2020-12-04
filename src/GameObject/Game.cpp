#include <vector>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Box2D/Box2D.h>
#include "Game.hpp"


Game::Game(sf::Int32 id, ClientService *clientService, Settings* settings, sf::RenderWindow* window, int laps, const std::string &playerCarType, std::string mapPath)
    : id_(id), clientService_(clientService), settings_(settings), window_(window), laps_(laps) {
    b2Vec2 g = b2Vec2(0,0);
    world_ = new b2World(g);

    // Set world contact event listener
    contactListener_ = new ContactListener(this, settings);
    world_->SetContactListener(contactListener_);

    map_ = new GameMap(1.0, -2, window);
    map_->LoadMapFile(mapPath, world_);
    noOfCheckpoints_ = map_->GetNumberOfRaceLines();

    playerCar_ = CreatePlayerCar(playerCarType);
    playerCar_->Accelerate(false);
    playerCar_->Brake(false);
    playerCar_->TurnLeft(false);
    playerCar_->TurnRight(false);
    playerCar_->SetTransform(b2Vec2(116, 107), -90*DEG_TO_RAD);
    RaceState *rs = new RaceState{0, -100};
    raceStates_.insert(std::pair<sf::Int32, RaceState*>(id, rs));

    Box *box = new Box(GenerateID(), "../res/smallcrate.png", world_, window);
    box->SetTransform(b2Vec2(20,30), 0.0);
    objects_.push_back(box);
    networkedObjects_.push_back(box);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(box->GetID(), box));
    TireStack *tireStack = new TireStack(GenerateID(), "../res/tirestack.png", world_, window);
    tireStack->SetTransform(b2Vec2(0,10), 0.0);
    objects_.push_back(tireStack);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(tireStack->GetID(), tireStack) );
    Oilspill *oilSpill = new Oilspill(401, "../res/oilspill.png", world_, window);
    oilSpill->SetTransform(b2Vec2(10,20), 0.0);
    objects_.push_back(oilSpill);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(oilSpill->GetID(), oilSpill) );
    Boost *boost = new Boost(301, "../res/boost.png", world_,window);
    boost->SetTransform(b2Vec2(0,5), 0.0);
    objects_.push_back(boost);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(boost->GetID(), boost) );

    // Sounds
    //sf::Listener::setDirection(0,0,1);
}


Game::~Game() {
    for(auto o: objects_) {
        delete o;
    }
    for(Tire *t: playerCar_->GetTires()) {
        delete t;
    }
    for(auto& rs: raceStates_) {
        delete rs.second;
    }
    delete playerCar_;
    delete contactListener_;
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
    // Spatialized sound
    sf::Listener::setPosition(playerCar_->GetTransform().p.x, 0.f, window_->getSize().y - playerCar_->GetTransform().p.y);
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

void Game::UpdateRaceState(sf::Int32 carId, sf::Int32 raceLineId) {
    //Check if the crossed checpoint is the next in line
    RaceState *carState = raceStates_[carId];
    if(carState->nextRaceLineId == raceLineId) {
        std::cout << "Crossed raceline "<< std::endl;
        carState->nextRaceLineId -= 1;

        //Finish line
        if(raceLineId == -100) {
            carState->currentLap += 1;
            if(carState->currentLap == laps_+1) {
                sf::Packet packet;
                packet << CLIENT_WIN;
                clientService_->Send(packet);
            }
        }
        //Last checkpoint
        if(raceLineId == -99 - noOfCheckpoints_) {
            carState->nextRaceLineId = -100;
        }
    }
}

void Game::GiveBoost(sf::Int32 carId, float boostScale) {
    Car *car = (Car*)objectMap_[carId];
    car->ApplyBoost(boostScale);
}
void Game::GiveSpin(sf::Int32 carId, float spinScale) {
    Car *car = (Car*)objectMap_[carId];
    car->ApplySpin(spinScale);
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
    Car* car = new Car(ids, world_, settings_->GetCarData(carType), window_);
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
    //Create local ids for the tires
    std::vector<sf::Int32> ids;
    ids.push_back(id);
    for (int i = 0; i < 4; i++)
    {
        ids.push_back(GenerateID());
    }

    //Create the car and add it to the necessary containers
    Car* car = new Car(ids, world_, settings_->GetCarData(carType), window_);  
    objects_.push_back(car);
    objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[0], car));

    //Add the tires to the necessary containers
    std::vector<Tire*> tires = car->GetTires();
    for(int i = 0; i < 4; i++) {
        objects_.push_back(tires[i]);
        objectMap_.insert(std::pair<sf::Int32, DynamicObject*>(ids[i+1], tires[i]));
    }

    car->isLocalPlayer_ = false;
    RaceState *rs = new RaceState{0, 0};
    raceStates_.insert(std::pair<sf::Int32, RaceState*>(id, rs));
    return car;
}

void Game::RemoveCar(sf::Int32 id)
{
    //TODO add exception handling
    Car *carToRemove = (Car*)objectMap_.at(id);
    std::cout << id << std::endl;
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

int Game::GetCurrentPlayerLap()
{
    return raceStates_[id_]->currentLap;
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
