#pragma once

#include <vector>
#include <memory>

#include "GameObject.hpp"
#include "DynamicObject.hpp"
#include "Car.hpp"
#include "CarData.hpp"
#include "GameMap.hpp"
#include "RaceLine.hpp"
#include "Box.hpp"
#include "RaceState.hpp"
#include "ContactListener.hpp"
class ClientService;
#include "../network/ClientService.hpp"
class ContactListener;
#include "RaceLine.hpp"

class Game {
public:
    Game(sf::Int32 id, ClientService *clientService, Settings* settings, int laps, const std::string &playerCarType, std::string mapPath);
    ~Game();

    std::vector<DynamicObject*> GetObjects();
    std::map<sf::Int32, GameObject*> GetObjectMap();
    std::vector<DynamicObject*> GetNetworkedObjects();
    std::vector<sf::Int32> GetPlayerCarIDs();
    Car *GetPlayerCar();
    GameMap *GetMap();

    void Init();
    void Update(float dt);
    void UpdateObject(sf::Int32 id, b2Transform transform, b2Vec2 velocity, float angularVelocity);
    void UpdateCar(sf::Int32 id, b2Transform transform, b2Vec2 velocity, float angularVelocity, float steeringAngle);
    void UpdateRaceState(sf::Int32 carId, sf::Int32 raceLineId);

    Car* CreatePlayerCar(const std::string &carType);
    Car* AddCar(sf::Int32 id, const std::string &carType);
    void RemoveCar(sf::Int32 id);
    bool ContainsCar(sf::Int32 id);
    int GetCurrentPlayerLap();
    sf::Int32 GenerateID();

    float GetFriction(b2Vec2 coords) const;

private:
    sf::Int32 id_;
    Car *playerCar_;
    b2World *world_;
    GameMap *map_;
    int noOfCheckpoints_;
    int laps_;
    Settings* settings_;
    ContactListener* contactListener_;
    ClientService *clientService_;
    //Maps player ids to their "RaceState"
    std::map<sf::Int32, RaceState*> raceStates_;
    //All objects in the world except player-car and its tires
    std::vector<DynamicObject*> objects_;
    //All objects in the world indexed by their ids
    std::map<sf::Int32, GameObject*> objectMap_;
    //All miscellaneous objects which are synced between clients
    std::vector<DynamicObject*> networkedObjects_;

    
};