#include "ContactListener.hpp"

void ContactListener::BeginContact(b2Contact *contact)
{
    GameObject *fixtureAObj = static_cast<GameObject*>(contact->GetFixtureA()->GetUserData());
    GameObject *fixtureBObj = static_cast<GameObject*>(contact->GetFixtureB()->GetUserData());
    sf::Int32 idA = -1;
    sf::Int32 idB = -1;

    if (fixtureAObj != NULL && fixtureBObj != NULL) {
        idA = fixtureAObj->GetID();
        idB = fixtureBObj->GetID();
    }

    float spinScale = 100;

    //Fixture A is a car
    if(0 <= idA && idA <= MAX_CLIENTS) {
        //Fixture B is a RaceLine
        if(idB <= -100) {
            game_->UpdateRaceState(idA, idB);
        }
        //Fixture B is a Boost
        else if(300 <= idB && idB <= 399)
        {
            float boostPower = static_cast<Boost*>(game_->GetObjectMap()[idB])->GetMagnitude();
            game_->GiveBoost(idA, boostPower);
        }
        //Fixture B is an OilSpill
        else if(400 <= idB && idB <= 499)
        {
            game_->GiveSpin(idA, spinScale);
        }
        else if(game_->GetPlayerCar()->GetID() == idA)
        {
            settings_->PlaySound("collisionsound");
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
            float boostPower = static_cast<Boost*>(game_->GetObjectMap()[idA])->GetMagnitude();
            game_->GiveBoost(idB, boostPower);
        }
        //Fixture A is an OilSpill
        else if(400 <= idA && idA <= 499)
        {
            game_->GiveSpin(idB, spinScale);
        }
        else if(game_->GetPlayerCar()->GetID() == idB)
        {
            settings_->PlaySound("collisionsound");
        }
    }
}