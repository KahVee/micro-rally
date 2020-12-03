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
        //Fixture B is a Boost
        else if(300 <= idB && idB <= 399)
        {
            float boostscale = 100;
            game_->GiveBoost(idA, boostscale);
        }
        //Fixture B is an OilSpill
        else if(400 <= idB && idB <= 499)
        {
            float spinscale = 100;
            game_->GiveSpin(idA, spinscale);
        }
    }
    //Fixture B is a car
    if(0 <= idB && idB <= MAX_CLIENTS) {
        //Fixture A is a RaceLine
        if(idA <= -100) {
            game_->UpdateRaceState(idB, idA);
        }
        //Fixture A is a Boost
        else if(300 <= idA && idA <= 399)
        {
            float boostscale = 100;
            game_->GiveBoost(idA, boostscale);
        }
        //Fixture A is an OilSpill
        else if(400 <= idA && idA <= 499)
        {
            float spinscale = 100;
            game_->GiveSpin(idA, spinscale);
        }
    }
}