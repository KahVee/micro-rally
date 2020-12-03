#include "ContactListener.hpp"

void ContactListener::BeginContact(b2Contact *contact)
{
    GameObject *fixtureAObj = static_cast<GameObject*>(contact->GetFixtureA()->GetUserData());
    GameObject *fixtureBObj = static_cast<GameObject*>(contact->GetFixtureB()->GetUserData());
    sf::Int32 idA, idB;

    if (fixtureAObj != NULL && fixtureBObj != NULL) {
        idA = fixtureAObj->GetID();
        idB = fixtureBObj->GetID();   
        std::cout << "Collision detected: ID: " << idA << ", ID2: " <<  idB << std::endl;
        settings_->PlaySound("collisionsound");   
    }

    //Fixture A is a car
    if(0 <= idA && idA <= MAX_CLIENTS) {
        //Fixture B is a RaceLine
        if(idB <= -100) {
            game_->UpdateRaceState(idA, idB);
        }
    }
    //Fixture B is a car
    if(0 <= idB && idB <= MAX_CLIENTS) {
        //Fixture A is a RaceLine
        if(idA <= -100) {
            game_->UpdateRaceState(idB, idA);
        }
    }
    //Fixture A is a car
    if(0 <= idA && idA <=MAX_CLIENTS)
    {
        //Fixture B is a Boost
        if(idB == 301)
        {
            std::cout << "boostiin tormays" << std::endl;
            float boostscale = 100;
            game_->GiveBoost(idA, boostscale);
        }
    }
    //Fixture B is a car
    if(0 <= idB && idB <=MAX_CLIENTS)
    {
        //Fixture A is a boost
        if(idA == 301)
        {
            std::cout << "boostiin tormays" << std::endl;
            float boostscale = 100;
            game_->GiveBoost(idA, boostscale);
        }
    }
        //Fixture A is a car
    if(0 <= idA && idA <=MAX_CLIENTS)
    {
        //Fixture B is an OilSpill
        if(idB == 401)
        {
            std::cout << "oilspilliin tormays" << std::endl;
            float spinscale = 100;
            game_->GiveSpin(idA, spinscale);
        }
    }
    //Fixture B is a car
    if(0 <= idB && idB <=MAX_CLIENTS)
    {
        //Fixture A is an Oilspill
        if(idA == 401)
        {
            std::cout << "oilspilliin tormays" << std::endl;
            float spinscale = 100;
            game_->GiveSpin(idA, spinscale);
        }
    }
}